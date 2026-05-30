#include "RotatorComponent.h"
#include <UndyneEngine/ECS/GameObject.h>

#include <cmath>
#include <glm/vec3.hpp>

namespace Digger
{
    RotatorComponent::RotatorComponent(float radius,
        float angularSpeed,
        float initialAngle) noexcept
        : m_Radius{ radius }
        , m_AngularSpeed{ angularSpeed }
        , m_Angle{ initialAngle }
    {
    }

    void RotatorComponent::update(float deltaTime)
    {
        m_Angle += m_AngularSpeed * deltaTime;

        const glm::vec3 newLocal{
            std::cos(m_Angle) * m_Radius,
            std::sin(m_Angle) * m_Radius,
            0.0f
        };

        getOwner()->getTransform().setLocalPosition(newLocal);
    }
}