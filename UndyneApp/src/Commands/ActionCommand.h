#ifndef ACTION_COMMAND_H
#define ACTION_COMMAND_H
#include <UndyneEngine.h>
#include "../GameState.h"
#include "../Components/ShootComponent.h"

namespace Digger
{
	class ActionCommand final : public UndyneEngine::Command
	{
	public:
		void execute() override
		{
			switch (GameState::phase())
			{
			case GameState::Phase::Menu:
				break;
			case GameState::Phase::Playing:
				if (UndyneEngine::Scene* scene = UndyneEngine::SceneManager::getActiveScene())
					if (UndyneEngine::GameObject* player = scene->findGameObjectByName("Player"))
						if (ShootComponent* shoot = player->getComponent<ShootComponent>())
							shoot->requestFire();
				break;
			case GameState::Phase::GameOver:
				GameState::returnToMenu();
				break;
			}
		}
	};
}
#endif
