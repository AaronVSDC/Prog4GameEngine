#include "ScoreComponent.h"
#include "GameObject.h"

namespace dae
{
    void ScoreComponent::AddPoints(int amount, Event cause)
    {
        m_Score += amount;
        Notify(GetOwner(), cause);
    }
}