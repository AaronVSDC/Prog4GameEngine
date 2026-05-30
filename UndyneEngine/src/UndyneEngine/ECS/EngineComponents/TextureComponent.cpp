#include "TextureComponent.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../GameObject.h"
#include "../../Renderer/Renderer.h"

namespace UndyneEngine
{
	TextureComponent::TextureComponent(const std::string& file)
	{
		m_Texture = ResourceManager::loadTexture(file);
	}
	void TextureComponent::render() const
	{
		const auto& pos = getOwner()->getTransform().getWorldPosition();
		Renderer::renderTexture(*m_Texture, pos.x, pos.y); 
	}
}