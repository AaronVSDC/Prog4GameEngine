#include <SDL3_ttf\SDL_ttf.h>
#include "Font.h"
#include "..\Log\Log.h"

namespace UndyneEngine
{

	TTF_Font* Font::getFont() const {
		return m_Font;
	}

	Font::Font(const std::string& fullPath, float size) : m_Font(nullptr)
	{
		m_Font = TTF_OpenFont(fullPath.c_str(), size);
		if (!m_Font)
			UDE_CORE_ERROR("Failed to load font: {}", SDL_GetError()); 
		
	}

	Font::~Font()
	{
		TTF_CloseFont(m_Font);
	}
}