#include "PointsDisplayComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include <string>

namespace dae
{
    void PointsDisplayComponent::Start()
    {
        UpdateDisplay();
    }

    void PointsDisplayComponent::OnNotify(GameObject* gameObject, const Event& event)
    {
        if (event == Event::ENEMY_DIED || event == Event::PICKUP_COLLECTED)
        {
            if (auto* score = gameObject->GetComponent<ScoreComponent>())
                m_DisplayedScore = score->GetScore(); 

            UpdateDisplay();
        }
    }

    void PointsDisplayComponent::UpdateDisplay()
    {
        if (auto* text = GetOwner()->GetComponent<TextComponent>())
            text->setText("Score: " + std::to_string(m_DisplayedScore));
    }
}