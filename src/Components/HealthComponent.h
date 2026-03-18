#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
    class HealthComponent final : public BaseComponent, public Subject
    {
    public:
        explicit HealthComponent(int lives = 3, int healthPerLife = 1)
            : m_Lives(lives), m_Health(healthPerLife), m_MaxHealth(healthPerLife) {
        }

        ~HealthComponent() override = default;

        void Update(float) override {}
        void Render() const override {}
        void Start() override {}

        void TakeDamage(int amount = 1);
        void Die();

        int GetLives()  const { return m_Lives; }
        int GetHealth() const { return m_Health; }
        bool IsDead()   const { return m_Lives <= 0; }

    private:
        int m_Lives;
        int m_Health;
        int m_MaxHealth;
    };
}
#endif