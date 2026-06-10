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
        explicit MoveCommand(glm::vec2 direction) noexcept
            : m_Direction{ direction }
        {
        }

        void execute() override
        {
            UndyneEngine::Scene* scene = UndyneEngine::SceneManager::getActiveScene();
            if (not scene)
                return;
            if (UndyneEngine::GameObject* player = scene->findGameObjectByName("Player"))
                if (MoveComponent* moveComponent = player->getComponent<MoveComponent>())
                    moveComponent->setDirection(m_Direction);
        }
    private:
        glm::vec2 m_Direction;
    };
}
#endif
