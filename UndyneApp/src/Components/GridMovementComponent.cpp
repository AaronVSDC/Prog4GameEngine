#include "GridMovementComponent.h"
#include "LevelGridComponent.h"
#include <UndyneEngine.h>

namespace Digger
{
	void GridMovementComponent::start()
	{
		m_Texture = getOwner()->getComponent<UndyneEngine::TextureComponent>();

		if (auto* scene = getOwner()->getScene())
			if (auto* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();
	}

	bool GridMovementComponent::step(glm::ivec2 direction, float deltaTime)
	{
		if (not m_Grid)
			return false;

		auto& transform = getOwner()->getTransform();
		const glm::vec3 position = transform.getLocalPosition();
		glm::vec2 center{ position.x, position.y };

		if (not m_Sliding)
		{
			if (direction == glm::ivec2{ 0, 0 })
				return false;
			const glm::ivec2 next = m_Grid->worldToCell(center) + direction;
			if (not m_Grid->inBounds(next) or not m_Grid->isDug(next))
				return false;
			m_SlideTarget = { m_Grid->laneCenterX(next.x), m_Grid->laneCenterY(next.y) };
			m_Sliding = true;
			faceDirection(direction);
		}

		const glm::vec2 toTarget = m_SlideTarget - center;
		const float distance = glm::length(toTarget);
		const float travel = m_Speed * deltaTime;
		if (distance <= travel)
		{
			center = m_SlideTarget;
			m_Sliding = false;
		}
		else
		{
			center += (toTarget / distance) * travel;
		}

		transform.setLocalPosition(center.x, center.y, 0.0f);
		return true;
	}

	void GridMovementComponent::faceDirection(glm::ivec2 direction)
	{
		if (not m_Texture or direction.x == 0)
			return;
		m_Texture->setFlip(direction.x < 0
			? UndyneEngine::Renderer::Flip::Horizontal
			: UndyneEngine::Renderer::Flip::None);
	}
}
