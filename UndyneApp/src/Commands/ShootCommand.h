#ifndef SHOOT_COMMAND_H
#define SHOOT_COMMAND_H
#include <UndyneEngine.h>
#include "../Components/ShootComponent.h"

namespace Digger
{
	class ShootCommand final : public UndyneEngine::Command
	{
	public:
		explicit ShootCommand(UndyneEngine::GameObject* gameObject) noexcept
			: m_GameObject{ gameObject }
		{
		}

		void execute() override
		{
			if (ShootComponent* shoot = m_GameObject->getComponent<ShootComponent>())
				shoot->requestFire();
			else
				UDE_WARN("ShootCommand execute(): GameObject has no ShootComponent.");
		}

	private:
		UndyneEngine::GameObject* m_GameObject;
	};
}
#endif
