#include "PickupComponent.h"
#include "LevelGridComponent.h"
#include "ScoreComponent.h"

namespace Digger
{
	PickupComponent::PickupComponent(int pointValue, glm::ivec2 cell) noexcept
		: m_PointValue{ pointValue }
		, m_Cell{ cell }
	{
	}

	void PickupComponent::start()
	{
		if (UndyneEngine::Scene* scene = getOwner()->getScene())
		{
			m_Player = scene->findGameObjectByName("Player");
			if (UndyneEngine::GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();
		}
	}

	void PickupComponent::update(float)
	{
		if (!m_Player or !m_Grid)
			return;

		const glm::vec3 playerPosition = m_Player->getTransform().getLocalPosition();
		const glm::ivec2 playerCell = m_Grid->worldToCell({ playerPosition.x, playerPosition.y });
		if (playerCell.x != m_Cell.x or playerCell.y != m_Cell.y)
			return;

		if (ScoreComponent* score = m_Player->getComponent<ScoreComponent>())
			score->addPoints(m_PointValue);

		getOwner()->markForRemoval();
	}
}
