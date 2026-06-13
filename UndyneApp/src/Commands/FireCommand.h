#ifndef FIRE_COMMAND_H
#define FIRE_COMMAND_H
#include <UndyneEngine.h>
#include "../Components/ShootComponent.h"

//std
#include <string>

namespace Digger
{
	class FireCommand final : public UndyneEngine::Command
	{
	public:
		explicit FireCommand(const std::string& targetName)
			: m_TargetName{ targetName }
		{
		}

		void execute() override
		{
			UndyneEngine::Scene* scene = UndyneEngine::SceneManager::getActiveScene();
			if (!scene)
				return;
			if (UndyneEngine::GameObject* target = scene->findGameObjectByName(m_TargetName))
				if (ShootComponent* shoot = target->getComponent<ShootComponent>())
					shoot->requestFire();
		}

	private:
		std::string m_TargetName;
	};
}
#endif
