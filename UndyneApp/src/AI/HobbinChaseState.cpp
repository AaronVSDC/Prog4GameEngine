#include "HobbinChaseState.h"
#include "NobbinChaseState.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/DigComponent.h"
#include "../Components/EnemyComponent.h"
#include "../GameState.h"
#include <UndyneEngine.h>

namespace Digger
{
	void HobbinChaseState::onEnter(UndyneEngine::GameObject& owner)
	{
		ChaseState::onEnter(owner);
		if (m_Mover)
			m_Mover->setSpeed(s_Speed);
		applyForm(owner, "Sprites/HobbinSprites.png", 3);
		if (m_Enemy)
			m_Enemy->setDeathTexture("Sprites/HobbinDeath.png");
		UDE_INFO("[{}] -> HobbinChase (Hobbin form, digging)", owner.getName());
	}

	std::unique_ptr<UndyneEngine::State> HobbinChaseState::update(UndyneEngine::GameObject& owner, float deltaTime)
	{
		if (GameState::isActionPaused())
			return nullptr;
		if (not m_Mover or not m_Digger)
			return nullptr;

		touchPlayer(owner);

		if (m_Mover->isAtCellCenter())
		{
			m_Direction = chooseDirection(owner, true);
			if (m_Direction != glm::ivec2{ 0, 0 } and not m_Digger->digToward(m_Direction))
			{
				m_Mover->step(m_Direction, deltaTime);
				return std::make_unique<NobbinChaseState>();
			}
		}

		m_Mover->step(m_Direction, deltaTime);
		return nullptr;
	}
}
