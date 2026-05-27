#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H
#include <UndyneEngine.h>

namespace Digger
{
    class MoveCommand final : public UndyneEngine::Command
    {
    public:
        MoveCommand(UndyneEngine::GameObject* obj, float dx, float dy) noexcept
            : m_Obj(obj), m_DX(dx), m_DY(dy) {
        }

        void execute(float value) override
        {
            auto& t = m_Obj->getTransform();
            const auto p = t.getLocalPosition();
            t.setLocalPosition(p.x + m_DX * value, p.y + m_DY * value, p.z);
        }
    private:
        UndyneEngine::GameObject* m_Obj;
        float m_DX, m_DY;
    };
}
#endif