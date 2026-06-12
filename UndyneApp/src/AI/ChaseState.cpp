#include "ChaseState.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/DigComponent.h"
#include "../Components/LevelGridComponent.h"
#include "../Components/LivesComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/MoveComponent.h"
#include <UndyneEngine.h>

//std
#include <cstdlib>

namespace Digger
{
	void ChaseState::onEnter(UndyneEngine::GameObject& owner)
	{
		m_Mover = owner.getComponent<GridMovementComponent>();
		m_Digger = owner.getComponent<DigComponent>();
		m_Enemy = owner.getComponent<EnemyComponent>();
		if (auto* scene = owner.getScene())
			if (auto* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();
	}

	glm::ivec2 ChaseState::chooseDirection(UndyneEngine::GameObject& owner, bool throughEarth) const
	{
		if (not m_Grid)
			return { 0, 0 };
		auto* scene = owner.getScene();
		if (not scene)
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
			if (not nearest or distanceSq < nearestDistanceSq)
			{
				nearest = digger;
				nearestDistanceSq = distanceSq;
			}
		}
		if (not nearest)
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
			if (throughEarth ? m_Grid->inBounds(next) : m_Grid->isOpen(next))
				return candidate;
		}
		return { 0, 0 };
	}

	void ChaseState::touchPlayer(UndyneEngine::GameObject& owner) const
	{
		if (not m_Grid)
			return;
		auto* scene = owner.getScene();
		if (not scene)
			return;

		UndyneEngine::GameObject* primary = scene->findGameObjectByName("Player");
		LivesComponent* lives = primary ? primary->getComponent<LivesComponent>() : nullptr;
		if (not lives)
			return;

		const glm::vec3 ownerPosition = owner.getTransform().getLocalPosition();
		const float reach = m_Grid->cellSize() * 0.5f;
		const float reachSq = reach * reach;

		for (UndyneEngine::GameObject* digger : scene->findGameObjectsWithComponent<MoveComponent>())
		{
			const glm::vec3 diggerPosition = digger->getTransform().getLocalPosition();
			const float deltaX = diggerPosition.x - ownerPosition.x;
			const float deltaY = diggerPosition.y - ownerPosition.y;
			if (deltaX * deltaX + deltaY * deltaY <= reachSq)
			{
				lives->die(*digger);
				return;
			}
		}
	}

	void ChaseState::applyForm(UndyneEngine::GameObject& owner, const std::string& texturePath, int columnCount) const
	{
		auto* texture = owner.getComponent<UndyneEngine::TextureComponent>();
		if (not texture)
			return;

		texture->setTexture(texturePath);
		if (m_Grid)
		{
			const float frameWidth = texture->getTextureSize().x / static_cast<float>(columnCount);
			texture->setScale(m_Grid->cellSize() * 0.7f / frameWidth);
		}
		if (auto* animation = owner.getComponent<UndyneEngine::AnimationComponent>())
			animation->setColumnCount(columnCount);
	}
}
