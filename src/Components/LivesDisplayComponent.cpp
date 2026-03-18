#include "LivesDisplayComponent.h"
#include "TextComponent.h"   // your existing text component
#include "GameObject.h"
#include <string>

namespace dae
{
    void LivesDisplayComponent::Start()
    {
        UpdateDisplay();
    }

    void LivesDisplayComponent::OnNotify(GameObject* /*gameObject*/, const Event& event)
    {
        if (event == Event::PLAYER_DIED)
        {
            --m_RemainingLives;
            if (m_RemainingLives < 0) m_RemainingLives = 0;
            UpdateDisplay();
        }
    }

    void LivesDisplayComponent::UpdateDisplay()
    {
        // Assumes the same GameObject has a TextComponent for rendering
        if (auto* text = GetOwner()->GetComponent<TextComponent>())
        {
            text->setText("Lives: " + std::to_string(m_RemainingLives));
        }
    }
}