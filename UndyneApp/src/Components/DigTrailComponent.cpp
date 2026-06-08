#include "DigTrailComponent.h"
#include "LevelGridComponent.h"
#include "UndyneEngine/Utils/Texture2D.h"

#include <glm/glm.hpp>

namespace Digger
{
	void DigTrailComponent::start()
	{
		m_Grid = getOwner()->getComponent<LevelGridComponent>();

		if (UndyneEngine::Scene* scene = getOwner()->getScene())
			m_Player = scene->findGameObjectByName("Player");

		m_Hole = UndyneEngine::ResourceManager::loadTexture("Sprites/Hole/PlayerHole.png");

		if (m_Grid)
		{
			m_Canvas = UndyneEngine::Renderer::createRenderTarget(
				static_cast<int>(m_Grid->columns() * m_Grid->cellSize()),
				static_cast<int>(m_Grid->rows() * m_Grid->cellSize()));

			UndyneEngine::Renderer::setRenderTarget(m_Canvas.get());      

			//make a trail over all pre dug cells
			for (int row = 0; row < m_Grid->rows(); ++row)
			{
				for (int column = 0; column < m_Grid->columns(); ++column)
				{
					if (!m_Grid->isDug({ column, row })) continue;

					const glm::vec2 center{ m_Grid->laneCenterX(column), m_Grid->laneCenterY(row) };
					drawBrush(center);

					if (m_Grid->isDug({ column + 1, row }))
						stampLine(center, { m_Grid->laneCenterX(column + 1), m_Grid->laneCenterY(row) });
					if (m_Grid->isDug({ column, row + 1 }))
						stampLine(center, { m_Grid->laneCenterX(column), m_Grid->laneCenterY(row + 1) });
				}
			}
		}
	}

	void DigTrailComponent::update(float)
	{
		if (!m_Grid or !m_Player or !m_Canvas or !m_Hole) return;

		const glm::vec3 position = m_Player->getTransform().getLocalPosition();
		const glm::vec2 current{ position.x, position.y };

		UndyneEngine::Renderer::setRenderTarget(m_Canvas.get());

		if (!m_HasStamped)
		{
			drawBrush(current);
			m_LastStampPos = current;
			m_HasStamped = true;
		}
		else
		{
			glm::vec2 delta = current - m_LastStampPos;
			float distance = glm::length(delta);
			if (distance > 0.0f)
			{
				const float spacing = m_Grid->cellSize() * m_StampSpacing;
				const glm::vec2 stepDirection = delta / distance;
				while (distance >= spacing)
				{
					m_LastStampPos += stepDirection * spacing;
					drawBrush(m_LastStampPos);
					distance -= spacing;
				}
			}
		}

		UndyneEngine::Renderer::setRenderTarget(nullptr);
	}

	void DigTrailComponent::drawBrush(glm::vec2 worldPos) const
	{
		const glm::vec2 origin = m_Grid->cellTopLeft(0, 0);
		const float brush = m_Grid->cellSize() * m_BrushScale;
		UndyneEngine::Renderer::renderTexture(*m_Hole,
			worldPos.x - origin.x - brush * 0.5f,
			worldPos.y - origin.y - brush * 0.5f,
			brush, brush);
	}
	void DigTrailComponent::stampLine(glm::vec2 from, glm::vec2 to) const
	{
		const float spacing = m_Grid->cellSize() * m_StampSpacing;
		const glm::vec2 delta = to - from;
		const float distance = glm::length(delta);
		if (distance <= 0.0f) return;

		const glm::vec2 stepDirection = delta / distance;
		for (float travelled = spacing; travelled < distance; travelled += spacing)
			drawBrush(from + stepDirection * travelled);   
	}

	void DigTrailComponent::render() const
	{
		if (!m_Canvas || !m_Grid) return;
		const glm::vec2 origin = m_Grid->cellTopLeft(0, 0);
		UndyneEngine::Renderer::renderTexture(*m_Canvas, origin.x, origin.y,
			m_Grid->columns() * m_Grid->cellSize(),
			m_Grid->rows() * m_Grid->cellSize());
	}
}
