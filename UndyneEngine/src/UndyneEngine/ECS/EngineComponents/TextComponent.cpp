#include "TextComponent.h"
#include "../GameObject.h"
#include "../../Renderer/Renderer.h"
#include "../../Utils/Texture2D.h"
#include "../../Utils/Font.h"

namespace UndyneEngine
{
	TextComponent::TextComponent(const std::string& text, Font* font, Renderer::Color color)
		: m_Text{ text }
		, m_Font{ font }
		, m_Color{ color }
	{
	}

	void TextComponent::setText(const std::string& text)
	{
		if (text == m_Text)
			return;
		m_Text = text;
		m_NeedsRebuild = true;
	}

	void TextComponent::setColor(Renderer::Color color)
	{
		m_Color = color;
		m_NeedsRebuild = true;
	}

	void TextComponent::update(float)
	{
		if (!m_NeedsRebuild)
			return;
		m_NeedsRebuild = false;

		if (!m_Font || m_Text.empty())
		{
			m_Texture.reset();
			return;
		}
		m_Texture = Renderer::createTextTexture(*m_Font, m_Text, m_Color);
	}

	void TextComponent::render() const
	{
		if (!m_Texture)
			return;
		const glm::vec3& worldPosition = getOwner()->getTransform().getWorldPosition();
		float x = worldPosition.x;
		float y = worldPosition.y;
		if (m_Centered)
		{
			const glm::vec2 size = m_Texture->getSize();
			x -= size.x * 0.5f;
			y -= size.y * 0.5f;
		}
		Renderer::renderTexture(*m_Texture, x, y);
	}
}
