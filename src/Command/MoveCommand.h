#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H
#include "Command.h"

namespace dae
{
    class MoveCommand final : public dae::Command
    {
    public:
        MoveCommand(dae::GameObject* obj, glm::vec2 dir, float speed)
            : m_Obj(obj), m_Dir(dir), m_Speed(speed) {
        }

        void execute() override
        {
            if (!m_Obj) return;

            glm::vec3 pos = m_Obj->GetLocalPosition();
            pos += glm::vec3(m_Dir * m_Speed, 0.f);

            m_Obj->SetLocalPosition(pos);
        }

    private:
        dae::GameObject* m_Obj{};
        glm::vec2 m_Dir{};
        float m_Speed{};
    };



}


#endif
