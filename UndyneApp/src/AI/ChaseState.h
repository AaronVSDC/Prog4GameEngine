#ifndef CHASE_STATE_H
#define CHASE_STATE_H
#include <UndyneEngine/StateMachine/State.h>
#include <glm/vec2.hpp>

//std
#include <memory>
#include <string>

namespace UndyneEngine { class GameObject; }

namespace Digger
{
	class GridMovementComponent;
	class DigComponent;
	class LevelGridComponent;
	class EnemyComponent;

	class ChaseState : public UndyneEngine::State
	{
	public:
		void onEnter(UndyneEngine::GameObject& owner) override;

	protected:
		glm::ivec2 chooseDirection(UndyneEngine::GameObject& owner, bool throughEarth) const;
		void touchPlayer(UndyneEngine::GameObject& owner) const;
		void applyForm(UndyneEngine::GameObject& owner, const std::string& texturePath, int columnCount) const;

		GridMovementComponent* m_Mover{ nullptr };
		DigComponent* m_Digger{ nullptr };
		LevelGridComponent* m_Grid{ nullptr };
		EnemyComponent* m_Enemy{ nullptr };
	};
}
#endif
