#include "TextureComponent.h"
#include "..\..\ResourceManager\ResourceManager.h"
#include "TransformComponent.h"
#include "..\GameObject.h"
#include "..\..\Renderer\Renderer.h"

namespace UndyneEngine
{
	TextureComponent::TextureComponent(const std::string& file)
	{
		m_pTexture = ResourceManager::loadTexture(file);
	}
	void TextureComponent::render() const
	{
		if (!m_pTexture) return; 
		const auto& pos = getOwner()->getComponent<TransformComponent>()->getPosition(); 
		Renderer::renderTexture(*m_pTexture, pos.x, pos.y); 
	}
}