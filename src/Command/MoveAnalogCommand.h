#ifndef MOVE_ANALOG_COMMAND_H
#define MOVE_ANALOG_COMMAND_H
#include "Command.h"
#include "GameObject.h"
//#include "ColliderComponent.h"
//#include "RigidBody2DComponent.h"

namespace dae
{
	class MoveAnalogCommand final : public AnalogCommand
	{
	public:
		MoveAnalogCommand(dae::GameObject* obj, const float speedUnitsPerSecond)
			: m_Obj(obj), m_SPEED(speedUnitsPerSecond) {
		}

		void execute(const glm::vec2& dir) override
		{
			if (!m_Obj) return;

			glm::vec3 pos = m_Obj->GetLocalPosition();
			pos.x += dir.x;
			pos.y += dir.y; 

			m_Obj->SetLocalPosition(pos);
		}
	private:
		dae::GameObject* m_Obj{};
		const float m_SPEED{};
	};

}
#endif