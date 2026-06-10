#include "ProjectileComponent.h"
#include "LevelGridComponent.h"
#include "EnemyComponent.h"
#include "../AI/DeadState.h"

//std
#include <memory>
#include <vector>

using namespace UndyneEngine;

namespace Digger
{
	ProjectileComponent::ProjectileComponent(glm::ivec2 direction) noexcept
		: m_Direction{ direction }
	{
	}

	void ProjectileComponent::start()
	{
		if (Scene* scene = getOwner()->getScene())
			if (GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();
		if (m_Grid)
		{
			m_Speed = m_Grid->cellSize() * 10.0f;
			m_MaxDistance = m_Grid->cellSize() * 8.0f;
		}
	}

	bool ProjectileComponent::hitEnemy(glm::ivec2 cell)
	{
		Scene* scene = getOwner()->getScene();
		if (not scene)
			return false;

		for (GameObject* enemy : scene->findGameObjectsWithComponent<EnemyComponent>())
		{
			EnemyComponent* enemyComponent = enemy->getComponent<EnemyComponent>();
			if (not enemyComponent or not enemyComponent->isAlive())
				continue;
			if (not m_Grid->isOnCell(*enemy, cell))
				continue;

			if (StateMachineComponent* machine = enemy->getComponent<StateMachineComponent>())
				machine->changeState(std::make_unique<DeadState>());
			return true;
		}
		return false;
	}

	void ProjectileComponent::update(float deltaTime)
	{
		if (not m_Grid)
		{
			getOwner()->markForRemoval();
			return;
		}

		auto& transform = getOwner()->getTransform();
		glm::vec3 position = transform.getLocalPosition();
		const float step = m_Speed * deltaTime;
		position.x += static_cast<float>(m_Direction.x) * step;
		position.y += static_cast<float>(m_Direction.y) * step;
		transform.setLocalPosition(position.x, position.y, 0.0f);

		m_TravelledDistance += step;
		if (m_TravelledDistance >= m_MaxDistance)
		{
			getOwner()->markForRemoval();
			return;
		}

		const glm::vec2 fieldTopLeft = m_Grid->cellTopLeft(0, 0);
		const glm::vec2 fieldBottomRight = m_Grid->cellTopLeft(m_Grid->columns(), m_Grid->rows());
		if (position.x < fieldTopLeft.x or position.x > fieldBottomRight.x
			or position.y < fieldTopLeft.y or position.y > fieldBottomRight.y)
		{
			getOwner()->markForRemoval();
			return;
		}

		const glm::ivec2 cell = m_Grid->worldToCell({ position.x, position.y });
		if (not m_Grid->isDug(cell))
		{
			getOwner()->markForRemoval();
			return;
		}

		if (hitEnemy(cell))
			getOwner()->markForRemoval();
	}
}
