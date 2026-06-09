#ifndef NOBBIN_CHASE_STATE_H
#define NOBBIN_CHASE_STATE_H
#include "ChaseState.h"

namespace Digger
{
	class NobbinChaseState final : public ChaseState
	{
	public:
		void onEnter(UndyneEngine::GameObject& owner) override;
		std::unique_ptr<UndyneEngine::State> update(UndyneEngine::GameObject& owner, float deltaTime) override;

	private:
		static constexpr float s_Speed{ 90.0f };
		static constexpr float s_StuckLimit{ 0.5f };
		float m_StuckTimer{ 0.0f };
	};
}
#endif
