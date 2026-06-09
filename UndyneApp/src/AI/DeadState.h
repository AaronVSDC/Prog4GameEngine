#ifndef DEAD_STATE_H
#define DEAD_STATE_H
#include <UndyneEngine/StateMachine/State.h>

//std
#include <memory>

namespace UndyneEngine { class GameObject; }

namespace Digger
{
	class DeadState final : public UndyneEngine::State
	{
	public:
		void onEnter(UndyneEngine::GameObject& owner) override;
		std::unique_ptr<UndyneEngine::State> update(UndyneEngine::GameObject& owner, float deltaTime) override;

	private:
		static constexpr float s_RemoveDelay{ 1.5f };
		float m_Timer{ s_RemoveDelay };
	};
}
#endif
