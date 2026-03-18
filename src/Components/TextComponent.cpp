#include "TextComponent.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font)
	: m_text(text), m_font(font.get()), m_textTexture(nullptr)
{
}

void dae::TextComponent::Update(float)
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(),0, color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);
		m_textTexture = std::make_unique<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = GetOwner()->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::setText(const std::string& text)
{
	m_text = text; 
	m_needsUpdate = true;
}

void dae::TextComponent::setPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}