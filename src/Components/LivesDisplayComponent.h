#ifndef LIVES_DISPLAY_COMPONENT_H
#define LIVES_DISPLAY_COMPONENT_H

#include "BaseComponent.h"
#include "Observer.h"
#include <string>

namespace dae
{
    class TextComponent; 

    // This component IS an Observer — it registers itself with the player's HealthComponent
    class LivesDisplayComponent final : public BaseComponent, public Observer
    {
    public:
        explicit LivesDisplayComponent(int startingLives = 3)
            : m_RemainingLives(startingLives) {
        }

        ~LivesDisplayComponent() override = default;

        void Start() override;
        void Update(float) override {}
        void Render() const override {}

        void OnNotify(GameObject* gameObject, const Event& event) override;

        int GetRemainingLives() const { return m_RemainingLives; }

    private:
        int m_RemainingLives;

        void UpdateDisplay();
    };
}
#endif