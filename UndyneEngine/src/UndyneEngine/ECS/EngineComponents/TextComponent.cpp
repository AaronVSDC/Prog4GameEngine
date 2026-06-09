#include "TextComponent.h"
#include "../GameObject.h"
#include "../../Renderer/Renderer.h"
#include "../../Utils/Texture2D.h"
#include "../../Utils/Font.h"

namespace UndyneEngine
{
	TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font, Renderer::Color color)
		: m_Text{ text }
		, m_Font{ std::move(font) }
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
		Renderer::renderTexture(*m_Texture, worldPosition.x, worldPosition.y);
	}
}
