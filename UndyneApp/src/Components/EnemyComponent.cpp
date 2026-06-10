#include "EnemyComponent.h"
#include "LevelGridComponent.h"

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
		if (not texture)
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
}
