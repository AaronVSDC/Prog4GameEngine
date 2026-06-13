#include "LevelController.h"
#include "PickupComponent.h"
#include "EnemyComponent.h"
#include "LivesComponent.h"
#include "MonsterSpawnerComponent.h"
#include "../GameState/GameState.h"
#include <UndyneEngine.h>

using namespace UndyneEngine;

namespace Digger
{
	void LevelController::start()
	{
		if (Scene* scene = getOwner()->getScene())
			m_HadGems = !scene->findGameObjectsWithComponent<PickupComponent>().empty();
	}

	void LevelController::update(float deltaTime)
	{
		if (m_Resolved)
		{
			if (m_WinPending)
			{
				m_WinTimer -= deltaTime;
				if (m_WinTimer <= 0.0f)
				{
					m_WinPending = false;
					GameState::winCurrentLevel();
				}
			}
			return;
		}

		Scene* scene = getOwner()->getScene();
		if (!scene)
			return;

		const auto players = scene->findGameObjectsWithComponent<LivesComponent>();
		bool everyoneGameOver = !players.empty();
		for (GameObject* player : players)
			if (LivesComponent* lives = player->getComponent<LivesComponent>())
				if (!lives->isGameOver())
				{
					everyoneGameOver = false;
					break;
				}
		if (everyoneGameOver)
		{
			m_Resolved = true;
			GameState::enterGameOver();
			return;
		}

		if (GameState::isActionPaused())
			return;

		const bool gemsCleared = m_HadGems and scene->findGameObjectsWithComponent<PickupComponent>().empty();

		bool enemiesCleared = false;
		if (GameObject* spawnerObject = scene->findGameObjectByName("MonsterSpawner"))
			if (MonsterSpawnerComponent* spawner = spawnerObject->getComponent<MonsterSpawnerComponent>())
				enemiesCleared = spawner->hasSpawnedAll()
					and scene->findGameObjectsWithComponent<EnemyComponent>().empty();

		if (gemsCleared or enemiesCleared)
		{
			m_Resolved = true;
			m_WinPending = true;
			m_WinTimer = WIN_DELAY;

			for (GameObject* enemy : scene->findGameObjectsWithComponent<EnemyComponent>())
				enemy->markForRemoval();
			if (GameObject* spawnerObject = scene->findGameObjectByName("MonsterSpawner"))
				spawnerObject->markForRemoval();
		}
	}
}
