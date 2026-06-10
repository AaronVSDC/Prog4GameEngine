#include "DeadState.h"
#include "../Components/EnemyComponent.h"
#include <UndyneEngine.h>

namespace Digger
{
	void DeadState::onEnter(UndyneEngine::GameObject& owner)
	{
		if (auto* enemy = owner.getComponent<EnemyComponent>())
		{
			enemy->markDead();
			enemy->showDeathSprite();
		}
		UDE_INFO("[{}] -> Dead", owner.getName());
	}

	std::unique_ptr<UndyneEngine::State> DeadState::update(UndyneEngine::GameObject& owner, float deltaTime)
	{
		m_Timer -= deltaTime;
		if (m_Timer <= 0.0f)
			owner.markForRemoval();
		return nullptr;
	}
}
