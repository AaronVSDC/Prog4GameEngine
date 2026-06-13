#include "ManualControlComponent.h"
#include "GridMovementComponent.h"
#include "EnemyComponent.h"
#include "../GameState/GameState.h"
#include <UndyneEngine.h>

using namespace UndyneEngine;

namespace Digger
{
	void ManualControlComponent::start()
	{
		m_Mover = getOwner()->getComponent<GridMovementComponent>();
		m_Enemy = getOwner()->getComponent<EnemyComponent>();
	}

	void ManualControlComponent::update(float deltaTime)
	{
		if (GameState::isActionPaused())
			return;
		if (!m_Mover or !m_Enemy or !m_Enemy->isAlive())
			return;

		const glm::ivec2 direction = m_Enemy->consumeInputDirection();
		m_Mover->step(direction, deltaTime);
		m_Enemy->touchPlayer();
	}
}
