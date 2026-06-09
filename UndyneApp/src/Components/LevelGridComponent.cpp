#include "LevelGridComponent.h"
#include "UndyneEngine/Utils/Texture2D.h"

//std
#include <algorithm>
#include <cmath>

namespace Digger
{
	LevelGridComponent::LevelGridComponent(int columns, int rows, float nativeCellSize, const std::vector<glm::ivec2>& dugCells)
		: m_Columns{ columns }
		, m_Rows{ rows }
	{
		int outputWidth = 0;
		int outputHeight = 0;
		UndyneEngine::Renderer::getOutputSize(outputWidth, outputHeight);

		constexpr float hudFraction = 0.12f;
		const float topMargin = static_cast<float>(outputHeight) * hudFraction;
		const float availableHeight = static_cast<float>(outputHeight) - topMargin;

		const int scale = std::max(1, static_cast<int>(std::floor(std::min(
			static_cast<float>(outputWidth) / (columns * nativeCellSize),
			availableHeight / (rows * nativeCellSize)))));
		m_CellSize = nativeCellSize * scale;

		m_OriginX = (outputWidth - columns * m_CellSize) * 0.5f;
		m_OriginY = topMargin + (availableHeight - rows * m_CellSize) * 0.5f;

		m_Cells.assign(static_cast<std::size_t>(columns) * rows, CellState::Earth);
		m_Objects.assign(static_cast<std::size_t>(columns) * rows, nullptr);
		for (const glm::ivec2& cell : dugCells)
			markDug(cell);

		m_Background = UndyneEngine::ResourceManager::loadTexture("Sprites/BackgroundLevel1.png");
	}

	void LevelGridComponent::render() const
	{
		for (int row = 0; row < m_Rows; ++row)
		{
			for (int column = 0; column < m_Columns; ++column)
			{
				const float x = m_OriginX + column * m_CellSize;
				const float y = m_OriginY + row * m_CellSize;

				if (m_Background)
					UndyneEngine::Renderer::renderTexture(*m_Background, x, y, m_CellSize, m_CellSize);
			}
		}
	}

	std::size_t LevelGridComponent::cellIndex(int column, int row) const noexcept
	{
		return static_cast<std::size_t>(row) * m_Columns + column;
	}

	glm::vec2 LevelGridComponent::cellTopLeft(int column, int row) const noexcept
	{
		return { m_OriginX + column * m_CellSize, m_OriginY + row * m_CellSize };
	}

	glm::ivec2 LevelGridComponent::worldToCell(glm::vec2 worldCenter) const noexcept
	{
		const int column = static_cast<int>(std::floor((worldCenter.x - m_OriginX) / m_CellSize));
		const int row = static_cast<int>(std::floor((worldCenter.y - m_OriginY) / m_CellSize));
		return { std::clamp(column, 0, m_Columns - 1), std::clamp(row, 0, m_Rows - 1) };
	}

	int LevelGridComponent::nearestColumn(float worldX) const noexcept
	{
		const int column = static_cast<int>(std::lround((worldX - m_OriginX) / m_CellSize - 0.5f));
		return std::clamp(column, 0, m_Columns - 1);
	}

	int LevelGridComponent::nearestRow(float worldY) const noexcept
	{
		const int row = static_cast<int>(std::lround((worldY - m_OriginY) / m_CellSize - 0.5f));
		return std::clamp(row, 0, m_Rows - 1);
	}

	float LevelGridComponent::laneCenterX(int column) const noexcept
	{
		return m_OriginX + (column + 0.5f) * m_CellSize;
	}

	float LevelGridComponent::laneCenterY(int row) const noexcept
	{
		return m_OriginY + (row + 0.5f) * m_CellSize;
	}

	glm::vec2 LevelGridComponent::clampCenter(glm::vec2 worldCenter) const noexcept
	{
		const float half = m_CellSize * 0.5f;
		const float minX = m_OriginX + half;
		const float maxX = m_OriginX + m_Columns * m_CellSize - half;
		const float minY = m_OriginY + half;
		const float maxY = m_OriginY + m_Rows * m_CellSize - half;
		return { std::clamp(worldCenter.x, minX, maxX),
				 std::clamp(worldCenter.y, minY, maxY) };
	}

	bool LevelGridComponent::inBounds(glm::ivec2 cell) const noexcept
	{
		return cell.x >= 0 && cell.x < m_Columns && cell.y >= 0 && cell.y < m_Rows;
	}

	bool LevelGridComponent::isDug(glm::ivec2 cell) const noexcept
	{
		if (!inBounds(cell)) return false;
		return m_Cells[cellIndex(cell.x, cell.y)] == CellState::Dug;
	}

	void LevelGridComponent::markDug(glm::ivec2 cell) noexcept
	{
		if (!inBounds(cell)) return;
		m_Cells[cellIndex(cell.x, cell.y)] = CellState::Dug;
	}

	UndyneEngine::GameObject* LevelGridComponent::objectAt(glm::ivec2 cell) const noexcept
	{
		if (not inBounds(cell)) return nullptr;
		return m_Objects[cellIndex(cell.x, cell.y)];
	}

	void LevelGridComponent::setObjectAt(glm::ivec2 cell, UndyneEngine::GameObject* object) noexcept
	{
		if (not inBounds(cell)) return;
		m_Objects[cellIndex(cell.x, cell.y)] = object;
	}

	void LevelGridComponent::clearObjectAt(glm::ivec2 cell) noexcept
	{
		if (not inBounds(cell)) return;
		m_Objects[cellIndex(cell.x, cell.y)] = nullptr;
	}
}
