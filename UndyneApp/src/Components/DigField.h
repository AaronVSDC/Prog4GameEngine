#ifndef DIG_FIELD_H
#define DIG_FIELD_H
#include <UndyneEngine.h>
#include <glm/glm.hpp>

//std
#include <vector>
#include <memory>
#include <cstdint>

namespace UndyneEngine { class Texture2D; }

namespace Digger
{
	// Scene-level owner of the playfield. Holds two parallel representations:
	//   - a logical grid (Earth / Dug) that movement and collision query,
	//   - a baked canvas texture: the earth background is painted onto it once at
	//     load, and the dug ribbon is stamped on top as the player carves tunnels.
	class DigField final : public UndyneEngine::BaseComponent
	{
	public:
		DigField(int columns, int rows, float nativeCellSize, float pixelScale,
			float originX, float originY);
		~DigField() override;

		void render() const override;

		//---------
		// geometry
		//---------
		float cellStride() const noexcept { return m_CellStride; }
		float pixelScale() const noexcept { return m_PixelScale; }
		int columns() const noexcept { return m_Columns; }
		int rows() const noexcept { return m_Rows; }

		glm::vec2 cellTopLeft(int column, int row) const noexcept;
		glm::ivec2 worldToCell(glm::vec2 worldCenter) const noexcept;
		glm::vec2 worldToNative(glm::vec2 worldPoint) const noexcept;

		int nearestColumn(float worldX) const noexcept;
		int nearestRow(float worldY) const noexcept;
		float laneCenterX(int column) const noexcept;
		float laneCenterY(int row) const noexcept;
		glm::vec2 clampCenter(glm::vec2 worldCenter) const noexcept;

		//----------
		// dig state
		//----------
		bool inBounds(glm::ivec2 cell) const noexcept;
		bool isDug(glm::ivec2 cell) const noexcept;
		void markDug(glm::ivec2 cell) noexcept;

		//-------
		// visual
		//-------
		// Carves one round hole into the canvas at a point on the player's path. It has
		// no facing, so overlapping stamps form smooth corridors and clean corners.
		void stampHole(glm::vec2 nativeCenter);

	private:
		enum class CellState : std::uint8_t { Earth, Dug };

		// Paints the earth background across the whole canvas once, so the field draws
		// itself instead of relying on a grid of separate background tile objects.
		void bakeBackground();

		// Assembles the round-hole stamp once from the Hole/ edge pieces (corridor +
		// caps + walls) into an off-screen texture, ready to be stamped along the path.
		void buildHoleBrush();

		// Hole stamp size as a fraction of a cell; below 1 so parallel tunnels keep a
		// thin earth wall between them.
		static constexpr float k_HoleDiameterFactor{ 0.9f };

		int m_Columns;
		int m_Rows;
		float m_NativeCellSize;
		float m_PixelScale;
		float m_OriginX;
		float m_OriginY;
		float m_CellStride;

		std::vector<CellState> m_Cells;
		std::unique_ptr<UndyneEngine::Texture2D> m_Canvas;
		std::unique_ptr<UndyneEngine::Texture2D> m_HoleBrush;
		std::shared_ptr<UndyneEngine::Texture2D> m_Background;
	};
}

#endif
