#ifndef LEVEL_GRID_COMPONENT_H
#define LEVEL_GRID_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/glm.hpp>

//std
#include <vector>
#include <memory>
#include <cstdint>
#include <cstddef>

namespace UndyneEngine { class Texture2D; }

namespace Digger
{

	class LevelGridComponent final : public UndyneEngine::BaseComponent
	{
	public:
		LevelGridComponent(int columns, int rows, float nativeCellSize, const std::vector<glm::ivec2>& dugCells);

		void render() const override; 

		//---------
		// geometry
		//---------
		int columns() const noexcept { return m_Columns; }
		int rows() const noexcept { return m_Rows; }
		float cellSize() const noexcept { return m_CellSize; }
		glm::vec2 cellTopLeft(int column, int row) const noexcept;

		int nearestColumn(float worldX) const noexcept;
		int nearestRow(float worldY) const noexcept;
		float laneCenterX(int column) const noexcept;
		float laneCenterY(int row) const noexcept;
		glm::vec2 clampCenter(glm::vec2 worldCenter) const noexcept;

		//----------
		// dig state
		//----------
		glm::ivec2 worldToCell(glm::vec2 worldCenter) const noexcept;
		bool inBounds(glm::ivec2 cell) const noexcept;
		bool isDug(glm::ivec2 cell) const noexcept;
		void markDug(glm::ivec2 cell) noexcept;

	private:
		enum class CellState : std::uint8_t { Earth, Dug };
		std::size_t cellIndex(int column, int row) const noexcept;

		int m_Columns;
		int m_Rows;
		float m_CellSize{ 0.0f };
		float m_OriginX{ 0.0f };
		float m_OriginY{ 0.0f };

		std::vector<CellState> m_Cells;
		std::shared_ptr<UndyneEngine::Texture2D> m_Background;
	};
}

#endif
