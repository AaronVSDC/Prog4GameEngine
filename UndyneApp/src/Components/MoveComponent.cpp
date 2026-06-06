#include "MoveComponent.h"


namespace Digger
{
    void MoveComponent::start()
    {
        m_TextureComponent = getOwner()->getComponent<UndyneEngine::TextureComponent>();
    }

    void MoveComponent::update(float elapsedSec)
    {
        auto& transform = getOwner()->getTransform();

        glm::vec3 pos = transform.getLocalPosition();

        glm::vec3 dir3 = glm::vec3(m_Direction, 0.0f);

        pos += dir3 * m_Speed * elapsedSec;

        transform.setLocalPosition(pos);

        if (m_TextureComponent)
        {
            if (m_Direction.x != 0.0f)
            {
                m_TextureComponent->setRotation(0.0f);
                m_TextureComponent->setFlip(m_Direction.x < 0.0f
                    ? UndyneEngine::Renderer::Flip::Horizontal
                    : UndyneEngine::Renderer::Flip::None);
            }
            else if (m_Direction.y != 0.0f)
            {
                m_TextureComponent->setFlip(UndyneEngine::Renderer::Flip::None);
                m_TextureComponent->setRotation(m_Direction.y > 0.0f ? 90.0f : -90.0f);
            }
        }

        m_Direction = glm::vec2{ 0.f, 0.f };
    }
}