#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>

namespace Digger
{
    // Fired on a keyboard / gamepad button event. Moves by a fixed delta each fire.
    class MoveCommand final : public UndyneEngine::Command
    {
    public:
        MoveCommand(UndyneEngine::GameObject* gameObject, float deltaX, float deltaY) noexcept
            : m_GameObject(gameObject), m_DeltaX(deltaX), m_DeltaY(deltaY) {
        }

        void execute() override
        {
            auto& transform = m_GameObject->getTransform();
            const auto position = transform.getLocalPosition();
            transform.setLocalPosition(position.x + m_DeltaX, position.y + m_DeltaY, position.z);
        }
    private:
        UndyneEngine::GameObject* m_GameObject;
        float m_DeltaX;
        float m_DeltaY;
    };

    // Fired every frame with the analog stick's (x, y) vector after deadzone scaling.
    // SDL's Y is positive when the stick is pushed down; flipped here so 'up' is positive.
    class StickMoveCommand final : public UndyneEngine::StickCommand
    {
    public:
        StickMoveCommand(UndyneEngine::GameObject* gameObject, float speed) noexcept
            : m_GameObject(gameObject), m_Speed(speed) {
        }

        void execute(glm::vec2 stickValue) override
        {
            auto& transform = m_GameObject->getTransform();
            const auto position = transform.getLocalPosition();
            transform.setLocalPosition(
                position.x + stickValue.x * m_Speed,
                position.y - stickValue.y * m_Speed,
                position.z
            );
        }
    private:
        UndyneEngine::GameObject* m_GameObject;
        float m_Speed;
    };
}
#endif
