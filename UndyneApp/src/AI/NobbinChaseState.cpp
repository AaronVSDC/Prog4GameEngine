#include "NobbinChaseState.h"
#include "HobbinChaseState.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/LevelGridComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/MoveComponent.h"
#include "../GameState/GameState.h"
#include <UndyneEngine.h>

#include <cstdlib>

namespace Digger
{
	void NobbinChaseState::onEnter(UndyneEngine::GameObject& owner)
	{
		m_Mover = owner.getComponent<GridMovementComponent>();
		m_Enemy = owner.getComponent<EnemyComponent>();
		if (auto* scene = owner.getScene())
			if (auto* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();

		if (m_Mover)
			m_Mover->setSpeed(SPEED);
		applyForm(owner);
		if (m_Enemy)
			m_Enemy->setDeathTexture("Sprites/NobbinDeath.png");
		UDE_INFO("[{}] -> NobbinChase (Nobbin form)", owner.getName());
	}

	std::unique_ptr<UndyneEngine::State> NobbinChaseState::update(UndyneEngine::GameObject& owner, float deltaTime)
	{
		if (GameState::isActionPaused())
			return nullptr;
		if (!m_Mover)
			return nullptr;

		if (m_Enemy)
			m_Enemy->touchPlayer();

		glm::ivec2 direction{ 0, 0 };
		if (m_Mover->isAtCellCenter())
			direction = chooseDirection(owner);

		if (m_Mover->step(direction, deltaTime))
		{
			m_StuckTimer = 0.0f;
			return nullptr;
		}

		m_StuckTimer += deltaTime;
		if (m_StuckTimer > STUCK_LIMIT)
			return std::make_unique<HobbinChaseState>();
		return nullptr;
	}

	glm::ivec2 NobbinChaseState::chooseDirection(UndyneEngine::GameObject& owner) const
	{
		if (!m_Grid)
			return { 0, 0 };
		auto* scene = owner.getScene();
		if (!scene)
			return { 0, 0 };

		const glm::vec3 ownerPosition = owner.getTransform().getLocalPosition();

		UndyneEngine::GameObject* nearest = nullptr;
		float nearestDistanceSq = 0.0f;
		for (UndyneEngine::GameObject* digger : scene->findGameObjectsWithComponent<MoveComponent>())
		{
			const glm::vec3 diggerPosition = digger->getTransform().getLocalPosition();
			const float deltaX = diggerPosition.x - ownerPosition.x;
			const float deltaY = diggerPosition.y - ownerPosition.y;
			const float distanceSq = deltaX * deltaX + deltaY * deltaY;
			if (!nearest or distanceSq < nearestDistanceSq)
			{
				nearest = digger;
				nearestDistanceSq = distanceSq;
			}
		}
		if (!nearest)
			return { 0, 0 };

		const glm::ivec2 fromCell = m_Grid->worldToCell({ ownerPosition.x, ownerPosition.y });
		const glm::vec3 playerPosition = nearest->getTransform().getLocalPosition();
		const glm::ivec2 targetCell = m_Grid->worldToCell({ playerPosition.x, playerPosition.y });
		const glm::ivec2 delta = targetCell - fromCell;

		glm::ivec2 primary{ 0, 0 };
		glm::ivec2 secondary{ 0, 0 };
		if (std::abs(delta.x) >= std::abs(delta.y))
		{
			if (delta.x != 0) primary = { delta.x > 0 ? 1 : -1, 0 };
			if (delta.y != 0) secondary = { 0, delta.y > 0 ? 1 : -1 };
		}
		else
		{
			if (delta.y != 0) primary = { 0, delta.y > 0 ? 1 : -1 };
			if (delta.x != 0) secondary = { delta.x > 0 ? 1 : -1, 0 };
		}

		for (const glm::ivec2 candidate : { primary, secondary })
		{
			if (candidate == glm::ivec2{ 0, 0 })
				continue;
			const glm::ivec2 next = fromCell + candidate;
			if (m_Grid->isOpen(next))
				return candidate;
		}
		return { 0, 0 };
	}

	void NobbinChaseState::applyForm(UndyneEngine::GameObject& owner) const
	{
		auto* texture = owner.getComponent<UndyneEngine::TextureComponent>();
		if (!texture)
			return;

		texture->setTexture("Sprites/NobbinSprites.png");
		if (m_Grid)
		{
			const float frameWidth = texture->getTextureSize().x / 4.0f;
			texture->setScale(m_Grid->cellSize() * 0.7f / frameWidth);
		}
		if (auto* animation = owner.getComponent<UndyneEngine::AnimationComponent>())
			animation->setColumnCount(4);
	}
}
