#include "HealthComponent.h"
#include "GameObject.h"

namespace dae
{
    void HealthComponent::TakeDamage(int amount)
    {
        m_Health -= amount;
        if (m_Health <= 0)
            Die();
    }

    void HealthComponent::Die()
    {
        --m_Lives;
        Notify(GetOwner(), Event::PLAYER_DIED);

        if (m_Lives > 0)
            m_Health = m_MaxHealth; 
    }
}