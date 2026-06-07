#include "MoveComponent.h"
#include "DigField.h"

//std
#include <cmath>

namespace Digger
{
    void MoveComponent::start()
    {
        m_TextureComponent = getOwner()->getComponent<UndyneEngine::TextureComponent>();
    }

    void MoveComponent::update(float elapsedSec)
    {
        // Input is sampled fresh every frame (keys bind on InputState::Down), so consume
        // the request now; an empty request means "stand still this frame".
        const glm::vec2 desired = m_DesiredDirection;
        m_DesiredDirection = glm::vec2{ 0.0f, 0.0f };

        if (desired.x == 0.0f && desired.y == 0.0f)
            return;
        if (!m_Field)
            return;

        auto& transform = getOwner()->getTransform();
        glm::vec3 position = transform.getLocalPosition();

        // The player's transform stores its centre, which rides the cell lanes directly.
        glm::vec2 center{ position.x, position.y };

        const float step = m_Speed * elapsedSec;

        // What we end up moving this frame. While still lining up with a lane it is the
        // centring nudge; once aligned it becomes the real travel along the requested axis.
        glm::vec2 movement{ 0.0f, 0.0f };

        if (desired.x != 0.0f)
        {
            // Want to travel horizontally: centre on the row lane first, then move across.
            const float laneCenter = m_Field->laneCenterY(m_Field->nearestRow(center.y));
            const float toLane = laneCenter - center.y;
            if (std::abs(toLane) > step)
            {
                movement.y = (toLane > 0.0f) ? step : -step;
                center.y += movement.y;
            }
            else
            {
                center.y = laneCenter;
                movement.x = desired.x * step;
                center.x += movement.x;
            }
        }
        else
        {
            // Want to travel vertically: centre on the column lane first, then move along it.
            const float laneCenter = m_Field->laneCenterX(m_Field->nearestColumn(center.x));
            const float toLane = laneCenter - center.x;
            if (std::abs(toLane) > step)
            {
                movement.x = (toLane > 0.0f) ? step : -step;
                center.x += movement.x;
            }
            else
            {
                center.x = laneCenter;
                movement.y = desired.y * step;
                center.y += movement.y;
            }
        }

        center = m_Field->clampCenter(center);

        position.x = center.x;
        position.y = center.y;
        transform.setLocalPosition(position);

        // Face the way we actually moved, so the carved ribbon lines up with the path
        // even during the brief centring nudge before a turn.
        if (m_TextureComponent && (movement.x != 0.0f || movement.y != 0.0f))
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