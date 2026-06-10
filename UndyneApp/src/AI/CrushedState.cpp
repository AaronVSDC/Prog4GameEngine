#include "CrushedState.h"
#include "../Components/LevelGridComponent.h"
#include "../Components/EnemyComponent.h"
#include <UndyneEngine.h>

//std
#include <algorithm>

namespace Digger
{
	void CrushedState::onEnter(UndyneEngine::GameObject& owner)
	{
		if (auto* scene = owner.getScene())
			if (auto* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();

		if (auto* enemy = owner.getComponent<EnemyComponent>())
		{
			enemy->markDead();
			enemy->showDeathSprite();
		}

		const glm::vec3 position = owner.getTransform().getLocalPosition();
		m_FallY = position.y;
		if (m_Grid)
		{
			m_Cell = m_Grid->worldToCell({ position.x, position.y });
			m_FallSpeed = m_Grid->cellSize() * 8.0f;
		}
		UDE_INFO("[{}] -> Crushed (falling death)", owner.getName());
	}

	std::unique_ptr<UndyneEngine::State> CrushedState::update(UndyneEngine::GameObject& owner, float deltaTime)
	{
		if (not m_Grid)
		{
			owner.markForRemoval();
			return nullptr;
		}

		m_FallY += m_FallSpeed * deltaTime;

		const glm::ivec2 cellNow = m_Grid->worldToCell({ m_Grid->laneCenterX(m_Cell.x), m_FallY });
		m_Cell.y = std::max(m_Cell.y, cellNow.y);

		const glm::ivec2 below{ m_Cell.x, m_Cell.y + 1 };
		const bool belowSolid = not m_Grid->inBounds(below) or not m_Grid->isDug(below) or m_Grid->objectAt(below) != nullptr;
		const float restY = m_Grid->laneCenterY(m_Cell.y);

		if (belowSolid and m_FallY >= restY)
		{
			owner.getTransform().setLocalPosition(m_Grid->laneCenterX(m_Cell.x), restY, 0.0f);
			owner.markForRemoval();
			return nullptr;
		}

		owner.getTransform().setLocalPosition(m_Grid->laneCenterX(m_Cell.x), m_FallY, 0.0f);
		return nullptr;
	}
}
