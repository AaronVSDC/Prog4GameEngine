#ifndef MANUAL_CONTROL_STATE_H
#define MANUAL_CONTROL_STATE_H
#include "ChaseState.h"

namespace Digger
{
	class ManualControlState final : public ChaseState
	{
	public:
		void onEnter(UndyneEngine::GameObject& owner) override;
		std::unique_ptr<UndyneEngine::State> update(UndyneEngine::GameObject& owner, float deltaTime) override;

	private:
		static constexpr float s_Speed{ 90.0f };
	};
}
#endif
