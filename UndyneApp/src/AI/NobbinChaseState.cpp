#include "NobbinChaseState.h"
#include "HobbinChaseState.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/EnemyComponent.h"
#include <UndyneEngine.h>

namespace Digger
{
	void NobbinChaseState::onEnter(UndyneEngine::GameObject& owner)
	{
		ChaseState::onEnter(owner);
		if (m_Mover)
			m_Mover->setSpeed(s_Speed);
		applyForm(owner, "Sprites/NobbinSprites.png", 4);
		if (m_Enemy)
			m_Enemy->setDeathTexture("Sprites/NobbinDeath.png");
		UDE_INFO("[{}] -> NobbinChase (Nobbin form)", owner.getName());
	}

	std::unique_ptr<UndyneEngine::State> NobbinChaseState::update(UndyneEngine::GameObject& owner, float deltaTime)
	{
		if (not m_Mover)
			return nullptr;

		touchPlayer(owner);

		glm::ivec2 direction{ 0, 0 };
		if (m_Mover->isAtCellCenter())
			direction = chooseDirection(owner, false);

		if (m_Mover->step(direction, deltaTime))
		{
			m_StuckTimer = 0.0f;
			return nullptr;
		}

		m_StuckTimer += deltaTime;
		if (m_StuckTimer > s_StuckLimit)
			return std::make_unique<HobbinChaseState>();
		return nullptr;
	}
}
