#include "Transform.h"
#include "../ECS/GameObject.h"

namespace UndyneEngine
{
	Transform::Transform(glm::vec3 localPosition):
		m_LocalPosition(localPosition)
	{
	}
	Transform::Transform(float localX, float localY, float localZ):
		m_LocalPosition{localX, localY , localZ}
	{
	}
	void Transform::setLocalPosition(const glm::vec3& localPosition) noexcept
	{
		if (localPosition == m_LocalPosition) return; 
		m_LocalPosition = localPosition; 
		setDirty(); 
	}
	void Transform::setLocalPosition(float x, float y, float z) noexcept
	{
		setLocalPosition(glm::vec3(x, y, z)); 
	}
	const glm::vec3& Transform::getWorldPosition()
	{
		if (m_IsDirty)
		{
			if (m_Owner and m_Owner->getParent())
				m_WorldPosition = m_Owner->getParent()->getTransform().getWorldPosition() + m_LocalPosition;
			else
				m_WorldPosition = m_LocalPosition; 
			m_IsDirty = false; 
		}
		return m_WorldPosition; 
	}
	void Transform::setDirty() noexcept
	{
		if (m_IsDirty) return; 
		m_IsDirty = true; 
		if (m_Owner)
			m_Owner->markChildrenTransformDirty(); 
	}
}