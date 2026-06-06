#include "TextureComponent.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../GameObject.h"
#include "../../Renderer/Renderer.h"
#include "../../Utils/Texture2D.h"

namespace UndyneEngine
{
	TextureComponent::TextureComponent(const std::string& file)
	{
		m_Texture = ResourceManager::loadTexture(file);
	}
	 
	glm::vec2 TextureComponent::getTextureSize() const
	{
		return m_Texture->getSize();
	}

	void TextureComponent::render() const
	{
		const auto& worldPosition = getOwner()->getTransform().getWorldPosition();
		if (m_SourceRect.has_value())
		{
			const glm::vec4& sourceRect = *m_SourceRect;
			Renderer::renderTexture(*m_Texture, Renderer::TextureRenderInfo{
				.source      = { { sourceRect.x, sourceRect.y }, { sourceRect.z, sourceRect.w } },
				.destination = { { worldPosition.x, worldPosition.y },
								 { sourceRect.z * m_Scale.x, sourceRect.w * m_Scale.y } },
				.flip        = m_Flip,
				.rotation    = m_Rotation });
		}
		else
		{
			const glm::vec2 textureSize = m_Texture->getSize();
			Renderer::renderTexture(*m_Texture,
				worldPosition.x, worldPosition.y,
				textureSize.x * m_Scale.x, textureSize.y * m_Scale.y);
		}
	}
}