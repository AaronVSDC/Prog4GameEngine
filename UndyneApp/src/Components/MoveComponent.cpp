#include "MoveComponent.h"
#include "LevelGridComponent.h"
#include "GoldBagComponent.h"

//std
#include <cmath>

namespace Digger
{
    void MoveComponent::start()
    {
        m_TextureComponent = getOwner()->getComponent<UndyneEngine::TextureComponent>();

        if (UndyneEngine::Scene* scene = getOwner()->getScene())
            if (UndyneEngine::GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
                m_Grid = gridObject->getComponent<LevelGridComponent>();

        const glm::vec3 position = getOwner()->getTransform().getLocalPosition();
        m_SpawnPosition = glm::vec2{ position.x, position.y };
    }

    void MoveComponent::respawn()
    {
        getOwner()->getTransform().setLocalPosition(m_SpawnPosition.x, m_SpawnPosition.y, 0.0f);
        m_DesiredDirection = glm::vec2{ 0.0f, 0.0f };
    }

    bool MoveComponent::tryEnterCell(glm::ivec2 cell, int directionX)
    {
        UndyneEngine::GameObject* object = m_Grid->objectAt(cell);
        if (not object)
            return true;

        GoldBagComponent* bag = object->getComponent<GoldBagComponent>();
        if (not bag)
            return true;
        if (bag->isGold())
            return true;

        if (bag->isRestingBag() and directionX != 0)
        {
            const glm::ivec2 beyond{ cell.x + directionX, cell.y };
            if (m_Grid->inBounds(beyond) and m_Grid->isDug(beyond) and m_Grid->objectAt(beyond) == nullptr)
            {
                bag->pushTo(beyond);
                return true;
            }
        }
        return false;
    }

    void MoveComponent::update(float elapsedSec)
    {
        if (not m_Enabled)
            return;

        const glm::vec2 desired = m_DesiredDirection;
        m_DesiredDirection = glm::vec2{ 0.0f, 0.0f };

        if (desired.x == 0.0f and desired.y == 0.0f)
            return;
        if (not m_Grid)
            return;

        auto& transform = getOwner()->getTransform();
        glm::vec3 position = transform.getLocalPosition();

        glm::vec2 center{ position.x, position.y };

        const float step = m_Speed * elapsedSec;
        glm::vec2 movement{ 0.0f, 0.0f };

        if (desired.x != 0.0f)
        {
            const float laneCenter = m_Grid->laneCenterY(m_Grid->nearestRow(center.y));
            const float toLane = laneCenter - center.y;
            if (std::abs(toLane) > step)
            {
                movement.y = (toLane > 0.0f) ? step : -step;
                center.y += movement.y;
            }
            else
            {
                center.y = laneCenter;
                const int directionX = (desired.x > 0.0f) ? 1 : -1;
                const glm::vec2 intended{ center.x + desired.x * step, center.y };
                const glm::ivec2 currentCell = m_Grid->worldToCell(center);
                const glm::ivec2 intendedCell = m_Grid->worldToCell(intended);
                if (intendedCell.x == currentCell.x or tryEnterCell(intendedCell, directionX))
                {
                    movement.x = desired.x * step;
                    center.x += movement.x;
                }
            }
        }
        else
        {
            const float laneCenter = m_Grid->laneCenterX(m_Grid->nearestColumn(center.x));
            const float toLane = laneCenter - center.x;
            if (std::abs(toLane) > step)
            {
                movement.x = (toLane > 0.0f) ? step : -step;
                center.x += movement.x;
            }
            else
            {
                center.x = laneCenter;
                const glm::vec2 intended{ center.x, center.y + desired.y * step };
                const glm::ivec2 currentCell = m_Grid->worldToCell(center);
                const glm::ivec2 intendedCell = m_Grid->worldToCell(intended);
                if (intendedCell.y == currentCell.y or tryEnterCell(intendedCell, 0))
                {
                    movement.y = desired.y * step;
                    center.y += movement.y;
                }
            }
        }

        center = m_Grid->clampCenter(center);

        position.x = center.x;
        position.y = center.y;
        transform.setLocalPosition(position);

        if (m_TextureComponent and (movement.x != 0.0f or movement.y != 0.0f))
        {
            if (movement.x != 0.0f)
            {
                m_TextureComponent->setRotation(0.0f);
                m_TextureComponent->setFlip(movement.x < 0.0f
                    ? UndyneEngine::Renderer::Flip::Horizontal
                    : UndyneEngine::Renderer::Flip::None);
            }
            else
            {
                m_TextureComponent->setFlip(UndyneEngine::Renderer::Flip::None);
                m_TextureComponent->setRotation(movement.y > 0.0f ? 90.0f : -90.0f);
            }
        }
    }
}
