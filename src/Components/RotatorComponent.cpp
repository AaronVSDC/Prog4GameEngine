#include "RotatorComponent.h"

#include "GameObject.h"

namespace dae
{
	void RotatorComponent::Start()
	{
		auto* go = GetOwner();
		glm::vec3 pos = go->GetLocalPosition();

		m_Center = { pos.x, pos.y };
	}

    void RotatorComponent::Update(float deltaTime)
    {
        auto* go = GetOwner();
        if (!go) return;

        m_Angle += m_Speed * deltaTime;

        glm::vec3 pos = go->GetLocalPosition();

        glm::vec2 p2{
            m_Center.x + m_Radius * std::cos(m_Angle),
            m_Center.y + m_Radius * std::sin(m_Angle)
        };

        go->SetLocalPosition({ p2.x, p2.y, pos.z });
    }
}