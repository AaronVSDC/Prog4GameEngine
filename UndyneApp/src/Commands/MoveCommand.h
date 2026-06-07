#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>
#include "../Components/MoveComponent.h"

namespace Digger
{
    class MoveCommand final : public UndyneEngine::Command
    {
    public:
        MoveCommand(UndyneEngine::GameObject* gameObject, glm::vec2 direction) noexcept
            : m_GameObject(gameObject), m_Direction{direction}
        {
        }

        void execute() override
        {
            if (m_GameObject->hasComponent<MoveComponent>())
            {
                auto* moveComponent = m_GameObject->getComponent<MoveComponent>();

                moveComponent->setDirection(m_Direction);
            }
            else
            {
                UDE_WARN("MoveCommand execute(): GameObject you are trying to bind MoveCommand to has no MoveComponent."); 
            }
        }
    private:
        UndyneEngine::GameObject* m_GameObject;
        glm::vec2 m_Direction; 
    };
}
#endif
