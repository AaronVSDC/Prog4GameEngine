#include "EnemyComponent.h"
#include "LevelGridComponent.h"
#include "MoveComponent.h"
#include "LivesComponent.h"

using namespace UndyneEngine;

namespace Digger
{
	void EnemyComponent::start()
	{
		if (Scene* scene = getOwner()->getScene())
			if (GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();
	}

	void EnemyComponent::showDeathSprite()
	{
		TextureComponent* texture = getOwner()->getComponent<TextureComponent>();
		if (!texture)
			return;

		texture->setTexture(m_DeathTexture);
		if (AnimationComponent* animation = getOwner()->getComponent<AnimationComponent>())
			animation->setColumnCount(1);
		else
			texture->clearSourceRect();

		if (m_Grid)
		{
			const float frameWidth = texture->getTextureSize().x;
			if (frameWidth > 0.0f)
				texture->setScale(m_Grid->cellSize() * 0.7f / frameWidth);
		}
	}

	void EnemyComponent::touchPlayer() const
	{
		if (!m_Grid)
			return;

		Scene* scene = getOwner()->getScene();
		if (!scene)
			return;

		GameObject* primary = scene->findGameObjectByName("Player");
		LivesComponent* lives = primary ? primary->getComponent<LivesComponent>() : nullptr;
		if (!lives)
			return;

		const glm::vec3 ownerPosition = getOwner()->getTransform().getLocalPosition();
		const float reach = m_Grid->cellSize() * 0.5f;
		const float reachSq = reach * reach;

		for (GameObject* digger : scene->findGameObjectsWithComponent<MoveComponent>())
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
}
