#ifndef SECOND_PLAYER_MOVE_COMMAND_H
#define SECOND_PLAYER_MOVE_COMMAND_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>
#include "../Components/MoveComponent.h"
#include "../Components/EnemyComponent.h"

namespace Digger
{
	class SecondPlayerMoveCommand final : public UndyneEngine::Command
	{
	public:
		explicit SecondPlayerMoveCommand(glm::vec2 direction) noexcept
			: m_Direction{ direction }
		{
		}

		void execute() override
		{
			UndyneEngine::Scene* scene = UndyneEngine::SceneManager::getActiveScene();
			if (not scene)
				return;

			if (UndyneEngine::GameObject* player2 = scene->findGameObjectByName("Player2"))
			{
				if (MoveComponent* move = player2->getComponent<MoveComponent>())
					move->setDirection(m_Direction);
				return;
			}

			for (UndyneEngine::GameObject* monster : scene->findGameObjectsWithComponent<EnemyComponent>())
			{
				EnemyComponent* enemy = monster->getComponent<EnemyComponent>();
				if (enemy and enemy->isManualControlled() and enemy->isAlive())
				{
					enemy->setInputDirection(glm::ivec2{ static_cast<int>(m_Direction.x), static_cast<int>(m_Direction.y) });
					return;
				}
			}
		}

	private:
		glm::vec2 m_Direction;
	};
}
#endif
