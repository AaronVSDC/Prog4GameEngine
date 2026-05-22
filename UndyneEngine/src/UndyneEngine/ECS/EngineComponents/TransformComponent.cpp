#include "TransformComponent.h"

namespace UndyneEngine
{
	TransformComponent::TransformComponent(glm::vec3 position):
		m_Position(position)
	{
	}
	TransformComponent::TransformComponent(float x, float y, float z):
		m_Position{x, y , z}
	{
	}
	void TransformComponent::setPosition(float x, float y, float z)
	{
		m_Position.x = x; 
		m_Position.y = y; 
		m_Position.z = z; 
	}

	void TransformComponent::setPosition(const glm::vec3& position)
	{
		m_Position = position; 
	}
}