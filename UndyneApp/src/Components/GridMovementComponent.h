#ifndef GRID_MOVEMENT_COMPONENT_H
#define GRID_MOVEMENT_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>

namespace Digger
{
	class LevelGridComponent;

	class GridMovementComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;

		bool step(glm::ivec2 direction, float deltaTime);
		bool isAtCellCenter() const noexcept { return !m_Sliding; }
		void setSpeed(float speed) noexcept { m_Speed = speed; }

	private:
		void faceDirection(glm::ivec2 direction);

		LevelGridComponent* m_Grid{ nullptr };
		UndyneEngine::TextureComponent* m_Texture{ nullptr };
		float m_Speed{ 90.0f };
		bool m_Sliding{ false };
		glm::vec2 m_SlideTarget{ 0.0f, 0.0f };
	};
}
#endif
