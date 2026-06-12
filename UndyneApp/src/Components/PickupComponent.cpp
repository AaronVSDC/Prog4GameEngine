#include "PickupComponent.h"
#include "LevelGridComponent.h"
#include "ScoreComponent.h"
#include "MoveComponent.h"

using namespace UndyneEngine;

namespace Digger
{
	PickupComponent::PickupComponent(glm::ivec2 cell) noexcept
		: m_Cell{ cell }
	{
	}

	void PickupComponent::start()
	{
		if (Scene* scene = getOwner()->getScene())
			if (GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();
	}

	void PickupComponent::update(float)
	{
		if (!m_Grid)
			return;
		Scene* scene = getOwner()->getScene();
		if (!scene)
			return;

		for (GameObject* digger : scene->findGameObjectsWithComponent<MoveComponent>())
			if (m_Grid->isOnCell(*digger, m_Cell))
			{
				if (ScoreComponent* score = digger->getComponent<ScoreComponent>())
					score->collectEmerald();
				getOwner()->markForRemoval();
				return;
			}
	}
}
