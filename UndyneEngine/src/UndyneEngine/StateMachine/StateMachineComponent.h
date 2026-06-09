#ifndef STATE_MACHINE_COMPONENT_H
#define STATE_MACHINE_COMPONENT_H
#include "State.h"
#include "../ECS/BaseComponent.h"

//std
#include <memory>

namespace UndyneEngine
{
	class StateMachineComponent final : public BaseComponent
	{
	public:
		explicit StateMachineComponent(std::unique_ptr<State> initialState);

		void start() override;
		void update(float deltaTime) override;

		void changeState(std::unique_ptr<State> nextState);

	private:
		std::unique_ptr<State> m_CurrentState;
	};
}
#endif
