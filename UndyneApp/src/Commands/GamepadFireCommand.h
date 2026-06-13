#ifndef GAMEPAD_FIRE_COMMAND_H
#define GAMEPAD_FIRE_COMMAND_H
#include <UndyneEngine.h>
#include "../GameState/GameState.h"
#include "../Components/ShootComponent.h"

namespace Digger
{
	class GamepadFireCommand final : public UndyneEngine::Command
	{
	public:
		void execute() override
		{
			UndyneEngine::Scene* scene = UndyneEngine::SceneManager::getActiveScene();
			if (!scene)
				return;

			UndyneEngine::GameObject* shooter = scene->findGameObjectByName("Player2");
			if (!shooter and GameState::mode() == GameState::Mode::Normal)
				shooter = scene->findGameObjectByName("Player");
			if (!shooter)
				return;

			if (ShootComponent* shoot = shooter->getComponent<ShootComponent>())
				shoot->requestFire();
		}
	};
}
#endif
