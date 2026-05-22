#include "GameObject.h"

namespace UndyneEngine
{
	unsigned int GameObject::s_NextID = 0;

	GameObject::GameObject(std::optional<std::string> name)
		: m_ID{ s_NextID++ },
		m_Name{name ? std::move(*name) : "GameObject_" + std::to_string(m_ID)}
	{}

	void GameObject::start()
	{
		for (const auto& component : m_Components)
		{
			component->start(); 
		}
	}
	void GameObject::update(float deltaTime)
	{
		for (const auto& component : m_Components)
		{
			component->update(deltaTime); 
		}
	}
	void GameObject::fixedUpdate(float fixedTimeStep)
	{
		for (const auto& component : m_Components)
		{
			component->fixedUpdate(fixedTimeStep); 
		}
	}
	void GameObject::render() const
	{
		for (const auto& component : m_Components)
		{
			component->render(); 
		}

	}


}