#ifndef ENEMY_COMPONENT_H
#define ENEMY_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>

//std
#include <string>

namespace Digger
{
	class LevelGridComponent;

	class EnemyComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;

		void setDeathTexture(const std::string& texturePath) { m_DeathTexture = texturePath; }

		bool isAlive() const noexcept { return m_Alive; }
		void markDead() noexcept { m_Alive = false; }

		void showDeathSprite();

		void markManualControlled() noexcept { m_ManualControlled = true; }
		bool isManualControlled() const noexcept { return m_ManualControlled; }

		void setInputDirection(glm::ivec2 direction) noexcept { m_InputDirection = direction; }
		glm::ivec2 consumeInputDirection() noexcept
		{
			const glm::ivec2 direction = m_InputDirection;
			m_InputDirection = { 0, 0 };
			return direction;
		}

	private:
		std::string m_DeathTexture{ "Sprites/NobbinDeath.png" };
		bool m_Alive{ true };
		bool m_ManualControlled{ false };
		glm::ivec2 m_InputDirection{ 0, 0 };
		LevelGridComponent* m_Grid{ nullptr };
	};
}
#endif
