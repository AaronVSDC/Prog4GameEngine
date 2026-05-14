#include "GameObject.h"

namespace UndyneEngine
{

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