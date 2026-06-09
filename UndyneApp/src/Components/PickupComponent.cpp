#include "PickupComponent.h"
#include "LevelGridComponent.h"
#include "ScoreComponent.h"

namespace Digger
{
	PickupComponent::PickupComponent(glm::ivec2 cell) noexcept
		: m_Cell{ cell }
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
		if (not m_Player or not m_Grid)
			return;
		if (not m_Grid->isOnCell(*m_Player, m_Cell))
			return;

		if (ScoreComponent* score = m_Player->getComponent<ScoreComponent>())
			score->collectEmerald();

		getOwner()->markForRemoval();
	}
}
