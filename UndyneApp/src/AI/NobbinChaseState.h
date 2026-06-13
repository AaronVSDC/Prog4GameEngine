#ifndef NOBBIN_CHASE_STATE_H
#define NOBBIN_CHASE_STATE_H
#include <UndyneEngine/StateMachine/State.h>
#include <glm/vec2.hpp>

#include <memory>

namespace UndyneEngine { class GameObject; }

namespace Digger
{
	class GridMovementComponent;
	class LevelGridComponent;
	class EnemyComponent;

	class NobbinChaseState final : public UndyneEngine::State
	{
	public:
		void onEnter(UndyneEngine::GameObject& owner) override;
		std::unique_ptr<UndyneEngine::State> update(UndyneEngine::GameObject& owner, float deltaTime) override;

	private:
		glm::ivec2 chooseDirection(UndyneEngine::GameObject& owner) const;
		void applyForm(UndyneEngine::GameObject& owner) const;

		const float SPEED{ 90.0f };
		const float STUCK_LIMIT{ 0.5f };

		GridMovementComponent* m_Mover{ nullptr };
		LevelGridComponent* m_Grid{ nullptr };
		EnemyComponent* m_Enemy{ nullptr };
		float m_StuckTimer{ 0.0f };
	};
}
#endif
