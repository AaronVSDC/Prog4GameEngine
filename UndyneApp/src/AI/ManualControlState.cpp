#include "ManualControlState.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/EnemyComponent.h"
#include "../GameState.h"
#include <UndyneEngine.h>

namespace Digger
{
	void ManualControlState::onEnter(UndyneEngine::GameObject& owner)
	{
		ChaseState::onEnter(owner);
		if (m_Mover)
			m_Mover->setSpeed(s_Speed);
		applyForm(owner, "Sprites/NobbinSprites.png", 4);
		if (m_Enemy)
			m_Enemy->setDeathTexture("Sprites/NobbinDeath.png");
		UDE_INFO("[{}] -> ManualControl (player driven)", owner.getName());
	}

	std::unique_ptr<UndyneEngine::State> ManualControlState::update(UndyneEngine::GameObject& owner, float deltaTime)
	{
		if (GameState::isActionPaused())
			return nullptr;
		if (not m_Mover or not m_Enemy)
			return nullptr;

		const glm::ivec2 direction = m_Enemy->consumeInputDirection();
		m_Mover->step(direction, deltaTime);
		touchPlayer(owner);
		return nullptr;
	}
}
