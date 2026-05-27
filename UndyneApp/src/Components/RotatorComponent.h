#ifndef ROTATOR_COMPONENT_H
#define ROTATOR_COMPONENT_H

#include <UndyneEngine.h>

namespace Digger
{
    class RotatorComponent final : public UndyneEngine::BaseComponent
    {
    public:
        RotatorComponent(float radius,
            float angularSpeedRadPerSec,
            float initialAngleRad = 0.0f) noexcept;

        void update(float deltaTime) override;

    private:
        float m_Radius;
        float m_AngularSpeed;
        float m_Angle;
    };
}

#endif