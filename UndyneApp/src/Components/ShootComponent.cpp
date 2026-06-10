#include "ShootComponent.h"
#include "MoveComponent.h"
#include "LevelGridComponent.h"
#include "ProjectileComponent.h"

//std
#include <memory>

using namespace UndyneEngine;

namespace Digger
{
	void ShootComponent::start()
	{
		m_Move = getOwner()->getComponent<MoveComponent>();

		if (Scene* scene = getOwner()->getScene())
			if (GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();
	}

	void ShootComponent::update(float deltaTime)
	{
		if (m_Cooldown > 0.0f)
			m_Cooldown -= deltaTime;

		const bool wantsToFire = m_WantsToFire;
		m_WantsToFire = false;
		if (wantsToFire and m_Cooldown <= 0.0f)
			fire();
	}

	void ShootComponent::fire()
	{
		Scene* scene = getOwner()->getScene();
		if (not scene or not m_Grid or not m_Move)
			return;

		const glm::ivec2 direction = m_Move->facing();
		if (direction == glm::ivec2{ 0, 0 })
			return;

		auto projectile = std::make_unique<GameObject>("Fireball");
		TextureComponent* texture = projectile->addComponent<TextureComponent>("Sprites/Fireball.png");
		projectile->addComponent<AnimationComponent>(3);
		texture->setCentered(true);

		const float frameSize = texture->getTextureSize().y;
		if (frameSize > 0.0f)
			texture->setScale(m_Grid->cellSize() * 0.5f / frameSize);

		const glm::vec3 position = getOwner()->getTransform().getLocalPosition();
		projectile->getTransform().setLocalPosition(position.x, position.y, 0.0f);
		projectile->addComponent<ProjectileComponent>(direction);

		scene->add(std::move(projectile));
		m_Cooldown = s_FireCooldown;
	}
}
