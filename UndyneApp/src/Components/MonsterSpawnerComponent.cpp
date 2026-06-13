#include "MonsterSpawnerComponent.h"
#include "LevelGridComponent.h"
#include "EnemyComponent.h"
#include "LivesComponent.h"
#include "GridMovementComponent.h"
#include "DigComponent.h"
#include "../AI/NobbinChaseState.h"
#include "ManualControlComponent.h"
#include "../GameState/GameState.h"

//std
#include <algorithm>
#include <string>
#include <vector>

using namespace UndyneEngine;

namespace Digger
{
	MonsterSpawnerComponent::~MonsterSpawnerComponent()
	{
		for (Subject* subject : m_LivesSubjects)
			subject->removeObserver(this);
	}

	void MonsterSpawnerComponent::configure(glm::ivec2 spawnCell, int totalForLevel, int maxAlive, float interval, bool manual) noexcept
	{
		m_SpawnCell = spawnCell;
		m_TotalForLevel = totalForLevel;
		m_MaxAlive = maxAlive;
		m_Interval = interval;
		m_Manual = manual;
	}

	void MonsterSpawnerComponent::start()
	{
		Scene* scene = getOwner()->getScene();
		if (!scene)
			return;

		if (GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
			m_Grid = gridObject->getComponent<LevelGridComponent>();

		for (GameObject* player : scene->findGameObjectsWithComponent<LivesComponent>())
			if (LivesComponent* lives = player->getComponent<LivesComponent>())
			{
				m_LivesSubjects.push_back(lives);
				lives->addObserver(this);
			}
	}

	void MonsterSpawnerComponent::update(float deltaTime)
	{
		if (GameState::isActionPaused())
			return;
		if (!m_Grid or hasSpawnedAll())
			return;

		m_Timer -= deltaTime;
		if (m_Timer > 0.0f)
			return;

		Scene* scene = getOwner()->getScene();
		if (!scene)
			return;

		int livingCount = 0;
		bool spawnCellBlocked = false;
		for (GameObject* monster : scene->findGameObjectsWithComponent<EnemyComponent>())
		{
			EnemyComponent* enemy = monster->getComponent<EnemyComponent>();
			if (!enemy or !enemy->isAlive())
				continue;
			++livingCount;
			if (m_Grid->isOnCell(*monster, m_SpawnCell))
				spawnCellBlocked = true;
		}

		if (livingCount >= m_MaxAlive or spawnCellBlocked)
			return;

		spawnMonster(*scene);
		m_Timer = m_Interval;
	}

	void MonsterSpawnerComponent::spawnMonster(Scene& scene)
	{
		const glm::vec2 worldPosition{ m_Grid->laneCenterX(m_SpawnCell.x), m_Grid->laneCenterY(m_SpawnCell.y) };
		const std::string name = "Monster_" + std::to_string(m_SpawnedCount);
		scene.add(createEnemy(name, worldPosition, m_Grid->cellSize(), m_Manual));
		++m_SpawnedCount;
	}

	std::unique_ptr<GameObject> MonsterSpawnerComponent::createEnemy(
		const std::string& name, glm::vec2 worldPosition, float cellSize, bool manual)
	{
		constexpr int columnCount = 4;
		constexpr float fillRatio = 0.7f;

		auto monster = std::make_unique<GameObject>(name);
		TextureComponent* texture = monster->addComponent<TextureComponent>("Sprites/NobbinSprites.png");
		monster->addComponent<AnimationComponent>(columnCount);
		texture->setCentered(true);

		const float frameWidth = texture->getTextureSize().x / static_cast<float>(columnCount);
		if (frameWidth > 0.0f)
			texture->setScale(cellSize * fillRatio / frameWidth);

		monster->getTransform().setLocalPosition(worldPosition.x, worldPosition.y, 0.0f);

		monster->addComponent<GridMovementComponent>();
		DigComponent* dig = monster->addComponent<DigComponent>();
		dig->setAutoDig(false);
		dig->setClearsObstacles(true);

		EnemyComponent* enemy = monster->addComponent<EnemyComponent>();
		if (manual)
		{
			enemy->markManualControlled();
			monster->addComponent<ManualControlComponent>();
			monster->addComponent<StateMachineComponent>();
		}
		else
		{
			monster->addComponent<StateMachineComponent>(std::make_unique<NobbinChaseState>());
		}

		return monster;
	}

	void MonsterSpawnerComponent::onNotify(GameObject&, Event event)
	{
		if (event == Event::PlayerDied)
			resetWave();
	}

	void MonsterSpawnerComponent::onSubjectDestroyed(Subject& subject)
	{
		std::erase(m_LivesSubjects, &subject);
	}

	void MonsterSpawnerComponent::resetWave()
	{
		int aliveCount = 0;
		if (Scene* scene = getOwner()->getScene())
		{
			for (GameObject* monster : scene->findGameObjectsWithComponent<EnemyComponent>())
			{
				EnemyComponent* enemy = monster->getComponent<EnemyComponent>();
				if (!enemy or !enemy->isAlive())
					continue;
				++aliveCount;
				monster->markForRemoval();
			}
		}

		m_SpawnedCount -= aliveCount;
		if (m_SpawnedCount < 0)
			m_SpawnedCount = 0;
		m_Timer = s_RespawnDelay;
	}
}
