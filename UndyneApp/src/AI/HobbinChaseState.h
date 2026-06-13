#ifndef HOBBIN_CHASE_STATE_H
#define HOBBIN_CHASE_STATE_H
#include <UndyneEngine/StateMachine/State.h>
#include <glm/vec2.hpp>

#include <memory>

namespace UndyneEngine { class GameObject; }

namespace Digger
{
	class GridMovementComponent;
	class DigComponent;
	class LevelGridComponent;
	class EnemyComponent;

	class HobbinChaseState final : public UndyneEngine::State
	{
	public:
		void onEnter(UndyneEngine::GameObject& owner) override;
		std::unique_ptr<UndyneEngine::State> update(UndyneEngine::GameObject& owner, float deltaTime) override;

	private:
		glm::ivec2 chooseDirection(UndyneEngine::GameObject& owner) const;
		void applyForm(UndyneEngine::GameObject& owner) const;

		const float SPEED{ 130.0f };

		GridMovementComponent* m_Mover{ nullptr };
		DigComponent* m_Digger{ nullptr };
		LevelGridComponent* m_Grid{ nullptr };
		EnemyComponent* m_Enemy{ nullptr };
		glm::ivec2 m_Direction{ 0, 0 };
	};
}
#endif
