#include "GameObject.h"

namespace UndyneEngine
{
	unsigned int GameObject::s_NextID = 0;

	GameObject::GameObject(std::optional<std::string> name)
		: m_ID{ s_NextID++ },
		m_Name{name ? std::move(*name) : "GameObject_" + std::to_string(m_ID)}
	{
		m_Transform.m_Owner = this; 
	}

	void GameObject::start()
	{
		for (const auto& component : m_Components)
			component->start(); 
	}
	void GameObject::update(float deltaTime)
	{
		for (const auto& component : m_Components)
			component->update(deltaTime); 
	}
	void GameObject::fixedUpdate(float fixedTimeStep)
	{
		for (const auto& component : m_Components)
			component->fixedUpdate(fixedTimeStep); 
	}
	void GameObject::render() const
	{
		for (const auto& component : m_Components)
			component->render(); 
	}
	void GameObject::setParent(GameObject* newParent, bool keepWorldPosition)
	{
		if (newParent == this) return;
		for (auto* parent = newParent; parent != nullptr; parent = parent->m_Parent)
			if (parent == this) return;

		if (m_Parent == newParent) return;

		if (keepWorldPosition)
		{
			const glm::vec3 worldNow = m_Transform.getWorldPosition();
			const glm::vec3 newParentWorld =
				newParent ? newParent->getTransform().getWorldPosition() : glm::vec3{ 0.0f };
			m_Transform.setLocalPosition(worldNow - newParentWorld);
		}
		else
		{
			m_Transform.setDirty();  
		}

		if (m_Parent) std::erase(m_Parent->m_Children, this);
		m_Parent = newParent;
		if (newParent) newParent->m_Children.emplace_back(this);
	}

	void GameObject::markChildrenTransformDirty()
	{
		for (auto* child : m_Children)
			child->m_Transform.setDirty(); 
	}

	void GameObject::markForRemoval() noexcept
	{
		m_MarkedForRemoval = true; 
		for (auto& child : m_Children)
			child->markForRemoval(); 
	}
}