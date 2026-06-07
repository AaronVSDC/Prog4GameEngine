#include "DigField.h"
#include "UndyneEngine/Utils/Texture2D.h"

//std
#include <algorithm>
#include <cmath>

namespace Digger
{
	DigField::DigField(int columns, int rows, float nativeCellSize, float pixelScale,
		float originX, float originY)
		: m_Columns{ columns }
		, m_Rows{ rows }
		, m_NativeCellSize{ nativeCellSize }
		, m_PixelScale{ pixelScale }
		, m_OriginX{ originX }
		, m_OriginY{ originY }
		, m_CellStride{ nativeCellSize * pixelScale }
	{
		m_Cells.assign(static_cast<std::size_t>(columns) * rows, CellState::Earth);

		const int canvasWidth = static_cast<int>(columns * nativeCellSize);
		const int canvasHeight = static_cast<int>(rows * nativeCellSize);
		m_Canvas = UndyneEngine::Renderer::createRenderTarget(canvasWidth, canvasHeight);
		m_Background = UndyneEngine::ResourceManager::loadTexture("Sprites/BackgroundLevel1.png");

		buildHoleBrush();
		bakeBackground();
	}

	DigField::~DigField() = default;

	void DigField::render() const
	{
		if (!m_Canvas) return;
		UndyneEngine::Renderer::renderTexture(*m_Canvas,
			m_OriginX, m_OriginY,
			m_Columns * m_CellStride, m_Rows * m_CellStride);
	}

	glm::vec2 DigField::cellTopLeft(int column, int row) const noexcept
	{
		return { m_OriginX + column * m_CellStride, m_OriginY + row * m_CellStride };
	}

	glm::ivec2 DigField::worldToCell(glm::vec2 worldCenter) const noexcept
	{
		const int column = static_cast<int>(std::floor((worldCenter.x - m_OriginX) / m_CellStride));
		const int row = static_cast<int>(std::floor((worldCenter.y - m_OriginY) / m_CellStride));
		return { std::clamp(column, 0, m_Columns - 1), std::clamp(row, 0, m_Rows - 1) };
	}

	glm::vec2 DigField::worldToNative(glm::vec2 worldPoint) const noexcept
	{
		return { (worldPoint.x - m_OriginX) / m_PixelScale,
				 (worldPoint.y - m_OriginY) / m_PixelScale };
	}

	int DigField::nearestColumn(float worldX) const noexcept
	{
		const int column = static_cast<int>(std::lround((worldX - m_OriginX) / m_CellStride - 0.5f));
		return std::clamp(column, 0, m_Columns - 1);
	}

	int DigField::nearestRow(float worldY) const noexcept
	{
		const int row = static_cast<int>(std::lround((worldY - m_OriginY) / m_CellStride - 0.5f));
		return std::clamp(row, 0, m_Rows - 1);
	}

	float DigField::laneCenterX(int column) const noexcept
	{
		return m_OriginX + (column + 0.5f) * m_CellStride;
	}

	float DigField::laneCenterY(int row) const noexcept
	{
		return m_OriginY + (row + 0.5f) * m_CellStride;
	}

	glm::vec2 DigField::clampCenter(glm::vec2 worldCenter) const noexcept
	{
		const float half = m_CellStride * 0.5f;
		const float minX = m_OriginX + half;
		const float maxX = m_OriginX + m_Columns * m_CellStride - half;
		const float minY = m_OriginY + half;
		const float maxY = m_OriginY + m_Rows * m_CellStride - half;
		return { std::clamp(worldCenter.x, minX, maxX),
				 std::clamp(worldCenter.y, minY, maxY) };
	}

	bool DigField::inBounds(glm::ivec2 cell) const noexcept
	{
		return cell.x >= 0 && cell.x < m_Columns && cell.y >= 0 && cell.y < m_Rows;
	}

	bool DigField::isDug(glm::ivec2 cell) const noexcept
	{
		if (!inBounds(cell)) return false;
		return m_Cells[static_cast<std::size_t>(cell.y) * m_Columns + cell.x] == CellState::Dug;
	}

	void DigField::markDug(glm::ivec2 cell) noexcept
	{
		if (!inBounds(cell)) return;
		m_Cells[static_cast<std::size_t>(cell.y) * m_Columns + cell.x] = CellState::Dug;
	}

	void DigField::buildHoleBrush()
	{
		auto capTop    = UndyneEngine::ResourceManager::loadTexture("Sprites/Hole/CapTop.png");
		auto capBottom = UndyneEngine::ResourceManager::loadTexture("Sprites/Hole/CapBottom.png");
		auto corridor  = UndyneEngine::ResourceManager::loadTexture("Sprites/Hole/CorridorHorizontal.png");
		auto wallLeft  = UndyneEngine::ResourceManager::loadTexture("Sprites/Hole/WallLeft.png");
		auto wallRight = UndyneEngine::ResourceManager::loadTexture("Sprites/Hole/WallRight.png");
		if (!capTop || !capBottom || !corridor || !wallLeft || !wallRight) return;

		const glm::vec2 capSize = capTop->getSize();
		const glm::vec2 corridorSize = corridor->getSize();
		const glm::vec2 capBottomSize = capBottom->getSize();
		const glm::vec2 wallLeftSize = wallLeft->getSize();
		const glm::vec2 wallRightSize = wallRight->getSize();

		const int brushWidth = static_cast<int>(capSize.x);
		const int brushHeight = static_cast<int>(capSize.y + corridorSize.y + capBottomSize.y);
		m_HoleBrush = UndyneEngine::Renderer::createRenderTarget(brushWidth, brushHeight);
		if (!m_HoleBrush) return;

		// Rough corridor down the middle, rounded caps top and bottom, rounded walls left
		// and right. The stamp has no facing, so sweeping it makes rounded tunnels and
		// clean corners.
		const auto drawPiece = [](UndyneEngine::Texture2D& piece, float x, float y, float width, float height)
		{
			const glm::vec2 size = piece.getSize();
			UndyneEngine::Renderer::renderTexture(piece, UndyneEngine::Renderer::TextureRenderInfo{
				.source = { { 0.0f, 0.0f }, { size.x, size.y } },
				.destination = { { x, y }, { width, height } } });
		};

		UndyneEngine::Renderer::setRenderTarget(m_HoleBrush.get());
		drawPiece(*corridor,  0.0f, capSize.y, static_cast<float>(brushWidth), corridorSize.y);
		drawPiece(*capTop,    0.0f, 0.0f, static_cast<float>(brushWidth), capSize.y);
		drawPiece(*capBottom, 0.0f, static_cast<float>(brushHeight) - capBottomSize.y, static_cast<float>(brushWidth), capBottomSize.y);
		drawPiece(*wallLeft,  0.0f, (static_cast<float>(brushHeight) - wallLeftSize.y) * 0.5f, wallLeftSize.x, wallLeftSize.y);
		drawPiece(*wallRight, static_cast<float>(brushWidth) - wallRightSize.x, (static_cast<float>(brushHeight) - wallRightSize.y) * 0.5f, wallRightSize.x, wallRightSize.y);
		UndyneEngine::Renderer::setRenderTarget(nullptr);
	}

	void DigField::stampHole(glm::vec2 nativeCenter)
	{
		if (!m_Canvas || !m_HoleBrush) return;

		// Draw the brush a little smaller than a cell, centred on the path point. Sweeping
		// it gives flat-walled corridors; its symmetry keeps corners and ends smooth.
		const glm::vec2 brushSize = m_HoleBrush->getSize();
		const float diameter = m_NativeCellSize * k_HoleDiameterFactor;
		UndyneEngine::Renderer::setRenderTarget(m_Canvas.get());
		UndyneEngine::Renderer::renderTexture(*m_HoleBrush, UndyneEngine::Renderer::TextureRenderInfo{
			.source = { { 0.0f, 0.0f }, { brushSize.x, brushSize.y } },
			.destination = { { nativeCenter.x - diameter * 0.5f, nativeCenter.y - diameter * 0.5f },
							 { diameter, diameter } } });
		UndyneEngine::Renderer::setRenderTarget(nullptr);
	}

	void DigField::bakeBackground()
	{
		if (!m_Canvas || !m_Background) return;

		// The canvas is in native pixels, so each cell is exactly m_NativeCellSize square.
		// Stretching the tile to that size keeps the field seamless whatever the art size.
		const glm::vec2 textureSize = m_Background->getSize();
		UndyneEngine::Renderer::setRenderTarget(m_Canvas.get());
		for (int row = 0; row < m_Rows; ++row)
		{
			for (int column = 0; column < m_Columns; ++column)
			{
				UndyneEngine::Renderer::renderTexture(*m_Background, UndyneEngine::Renderer::TextureRenderInfo{
					.source = { { 0.0f, 0.0f }, { textureSize.x, textureSize.y } },
					.destination = { { column * m_NativeCellSize, row * m_NativeCellSize },
									 { m_NativeCellSize, m_NativeCellSize } } });
			}
		}
		UndyneEngine::Renderer::setRenderTarget(nullptr);
	}
}
