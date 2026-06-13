#include "DigComponent.h"
#include "LevelGridComponent.h"
#include "DigTrailComponent.h"

#include <glm/glm.hpp>

//std
#include <cmath>

namespace Digger
{
	void DigComponent::start()
	{
		if (auto* scene = getOwner()->getScene())
		{
			if (auto* gridObject = scene->findGameObjectByName("LevelGrid"))
			{
				m_Grid = gridObject->getComponent<LevelGridComponent>();
				if (auto* trail = gridObject->getComponent<DigTrailComponent>())
					trail->addFollower(getOwner()->getName());
			}
			else
			{
				UDE_WARN("Didn't find any object called \"LevelGrid\" in scene");
			}
		}
	}

	void DigComponent::update(float)
	{
		if (!m_AutoDig or !m_Grid) return;

		const glm::vec3 position = getOwner()->getTransform().getLocalPosition();
		const glm::vec2 center{ position.x, position.y };
		const glm::ivec2 cell = m_Grid->worldToCell(center);

		const glm::vec2 cellCenter{ m_Grid->laneCenterX(cell.x), m_Grid->laneCenterY(cell.y) };
		const float maxOffset = m_Grid->cellSize() * m_VisitMargin;
		if (std::abs(center.x - cellCenter.x) <= maxOffset and
			std::abs(center.y - cellCenter.y) <= maxOffset)
		{
			dig(cell);
		}
	}

	bool DigComponent::dig(glm::ivec2 cell)
	{
		if (!m_Grid or !m_Grid->inBounds(cell))
			return false;

		const bool wasEarth = !m_Grid->isDug(cell);
		m_Grid->markDug(cell);

		if (m_ClearsObstacles)
		{
			if (UndyneEngine::GameObject* occupant = m_Grid->objectAt(cell))
			{
				occupant->markForRemoval();
				m_Grid->clearObjectAt(cell);
			}
		}
		return wasEarth;
	}

	bool DigComponent::digToward(glm::ivec2 direction)
	{
		if (!m_Grid)
			return false;

		const glm::vec3 position = getOwner()->getTransform().getLocalPosition();
		const glm::ivec2 cell = m_Grid->worldToCell({ position.x, position.y }) + direction;
		return dig(cell);
	}
}
