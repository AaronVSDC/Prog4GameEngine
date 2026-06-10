#include "LevelController.h"
#include "PickupComponent.h"
#include "EnemyComponent.h"
#include "LivesComponent.h"
#include "MonsterSpawnerComponent.h"
#include "../GameState.h"
#include <UndyneEngine.h>

using namespace UndyneEngine;

namespace Digger
{
	void LevelController::start()
	{
		if (Scene* scene = getOwner()->getScene())
			m_HadGems = not scene->findGameObjectsWithComponent<PickupComponent>().empty();
	}

	void LevelController::update(float)
	{
		if (m_Resolved)
			return;

		Scene* scene = getOwner()->getScene();
		if (not scene)
			return;

		if (GameObject* player = scene->findGameObjectByName("Player"))
			if (LivesComponent* lives = player->getComponent<LivesComponent>())
				if (lives->isGameOver())
				{
					m_Resolved = true;
					GameState::enterGameOver();
					return;
				}

		const bool gemsCleared = m_HadGems and scene->findGameObjectsWithComponent<PickupComponent>().empty();

		bool enemiesCleared = false;
		if (GameObject* spawnerObject = scene->findGameObjectByName("MonsterSpawner"))
			if (MonsterSpawnerComponent* spawner = spawnerObject->getComponent<MonsterSpawnerComponent>())
				enemiesCleared = spawner->hasSpawnedAll()
					and scene->findGameObjectsWithComponent<EnemyComponent>().empty();

		if (gemsCleared or enemiesCleared)
		{
			m_Resolved = true;
			GameState::winCurrentLevel();
		}
	}
}
