#ifndef MOVE_COMPONENT_H
#define MOVE_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/glm.hpp>

namespace Digger
{
	class LevelGridComponent;

	class MoveComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float elapsedSec) override;

		// Records the direction the player wants to travel next update. The component
		// lines the player up with the grid before committing to a perpendicular turn,
		// so it never jumps across a lane.
		void setDirection(const glm::vec2& direction) { m_DesiredDirection = direction; }
		void setEnabled(bool enabled) noexcept { m_Enabled = enabled; }
		void respawn();

	private:
		bool tryEnterCell(glm::ivec2 cell, int directionX);

		float m_Speed = 200.f;
		glm::vec2 m_DesiredDirection{ 0.0f, 0.0f };
		glm::vec2 m_SpawnPosition{ 0.0f, 0.0f };
		bool m_Enabled{ true };
		UndyneEngine::TextureComponent* m_TextureComponent{ nullptr };
		const LevelGridComponent* m_Grid{ nullptr };
	};
}


#endif
