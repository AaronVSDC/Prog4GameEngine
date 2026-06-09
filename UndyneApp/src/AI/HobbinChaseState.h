#ifndef HOBBIN_CHASE_STATE_H
#define HOBBIN_CHASE_STATE_H
#include "ChaseState.h"
#include <glm/vec2.hpp>

namespace Digger
{
	class HobbinChaseState final : public ChaseState
	{
	public:
		void onEnter(UndyneEngine::GameObject& owner) override;
		std::unique_ptr<UndyneEngine::State> update(UndyneEngine::GameObject& owner, float deltaTime) override;

	private:
		static constexpr float s_Speed{ 130.0f };
		glm::ivec2 m_Direction{ 0, 0 };
	};
}
#endif
