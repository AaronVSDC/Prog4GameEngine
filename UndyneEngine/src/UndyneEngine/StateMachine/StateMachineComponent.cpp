#include "StateMachineComponent.h"
#include "../ECS/GameObject.h"

//std
#include <utility>

namespace UndyneEngine
{
	StateMachineComponent::StateMachineComponent(std::unique_ptr<State> initialState)
		: m_CurrentState{ std::move(initialState) }
	{
	}

	void StateMachineComponent::start()
	{
		if (m_CurrentState)
			m_CurrentState->onEnter(*getOwner());
	}

	void StateMachineComponent::update(float deltaTime)
	{
		if (!m_CurrentState)
			return;

		std::unique_ptr<State> nextState = m_CurrentState->update(*getOwner(), deltaTime);
		if (nextState)
			changeState(std::move(nextState));
	}

	void StateMachineComponent::changeState(std::unique_ptr<State> nextState)
	{
		if (m_CurrentState)
			m_CurrentState->onExit(*getOwner());

		m_CurrentState = std::move(nextState);

		if (m_CurrentState)
			m_CurrentState->onEnter(*getOwner());
	}
}
