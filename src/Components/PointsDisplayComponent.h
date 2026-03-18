#ifndef POINTS_DISPLAY_COMPONENT_H
#define POINTS_DISPLAY_COMPONENT_H

#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class PointsDisplayComponent final : public BaseComponent, public Observer
    {
    public:
        PointsDisplayComponent() = default;
        ~PointsDisplayComponent() override = default;

        PointsDisplayComponent(const PointsDisplayComponent&) = delete;
        PointsDisplayComponent(PointsDisplayComponent&&) = delete;
        PointsDisplayComponent& operator=(const PointsDisplayComponent&) = delete;
        PointsDisplayComponent& operator=(PointsDisplayComponent&&) = delete;

        void Start()          override;
        void Update(float)    override {}
        void Render()   const override {}

        void OnNotify(GameObject* gameObject, const Event& event) override;

    private:
        int m_DisplayedScore{ 0 };

        void UpdateDisplay();
    };
}
#endif