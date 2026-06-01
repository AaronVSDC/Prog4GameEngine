#include "SpriteComponent.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../GameObject.h"
#include "../../Renderer/Renderer.h"

namespace UndyneEngine
{
    SpriteComponent::SpriteComponent(const std::string& file,
        const glm::vec4& sourceRect,
        const glm::vec2& destinationSize)
        : m_SourceRect(sourceRect)
        , m_DestinationSize(destinationSize)
    {
        m_Texture = ResourceManager::loadTexture(file);
    }

    void SpriteComponent::render() const
    {
        const auto& worldPosition = getOwner()->getTransform().getWorldPosition();
        Renderer::renderTexture(*m_Texture,
            worldPosition.x, worldPosition.y, m_DestinationSize.x, m_DestinationSize.y,
            m_SourceRect.x, m_SourceRect.y, m_SourceRect.z, m_SourceRect.w);
    }
}