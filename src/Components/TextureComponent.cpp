#include "TextureComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

namespace dae
{
	TextureComponent::TextureComponent(const std::string& fileName)
		:m_FileName(fileName)
	{
		m_pTexture = ResourceManager::GetInstance().LoadTexture(m_FileName).get();

	}

	void TextureComponent::Render() const
	{
		auto pos = GetOwner()->GetTransform().GetPosition();
		const auto rotEuler = GetOwner()->GetTransform().GetRotationEuler();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}

	glm::vec2 TextureComponent::getSize() const
	{ 
		return m_pTexture->GetSize();
	}
}