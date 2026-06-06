#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Texture2D.h"
#include "../Renderer/Renderer.h"
#include "../Log/Log.h"

namespace UndyneEngine
{

	SDL_Texture* Texture2D::getSDLTexture() const
	{
		return m_Texture;
	}

	Texture2D::Texture2D(SDL_Texture* texture) : m_Texture{texture}
	{
		if (!m_Texture) UDE_CORE_ERROR("Texture2D cannot be nullptr!"); 
	}

	Texture2D::Texture2D(const std::string& fullPath)
	{
		SDL_Surface* surface = IMG_Load(fullPath.c_str());
		if (!surface)
			UDE_CORE_ERROR("Texture2D: Failed to load PNG: {}", SDL_GetError()); 
		

		m_Texture = SDL_CreateTextureFromSurface(
			Renderer::getSDLRenderer(),
			surface
		);

		SDL_DestroySurface(surface);

		if (!m_Texture)
		{
			UDE_CORE_ERROR("Texture2D: Failed to create texture from surface: {}", SDL_GetError());
			return;
		}

		SDL_SetTextureScaleMode(m_Texture, SDL_SCALEMODE_NEAREST);
		
	}

	Texture2D::~Texture2D()
	{
		SDL_DestroyTexture(m_Texture);
	}

	glm::vec2 Texture2D::getSize() const
	{
		float w{}, h{};
		SDL_GetTextureSize(m_Texture, &w, &h);
		return { w, h };
	}

}