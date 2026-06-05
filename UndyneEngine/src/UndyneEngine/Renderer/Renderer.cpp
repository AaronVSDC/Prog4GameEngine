#include <SDL3/SDL.h>
#include "Renderer.h"
#include "../Log/Log.h"
#include "../ECS/Scene/SceneManager.h"
#include "../Utils/Texture2D.h"

namespace UndyneEngine::Renderer
{
	namespace
	{
		SDL_Renderer* s_Renderer{};
		SDL_Window* s_Window{};
		SDL_Color s_ClearColor{};
	}
	void init(SDL_Window* window)
	{
		s_Window = window;
		SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
		s_Renderer = SDL_CreateRenderer(window, nullptr);
		if (!s_Renderer) {
			UDE_CORE_ERROR("Failed to create SDL renderer: {}", SDL_GetError()); 
			return; 
		}
	}
	void render()
	{
		SDL_SetRenderDrawColor(s_Renderer, s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a); 
		SDL_RenderClear(s_Renderer); 

		SceneManager::render(); 

		SDL_RenderPresent(s_Renderer); 
	}
	void destroy()
	{
		if (!s_Renderer)
		{
			SDL_DestroyRenderer(s_Renderer); 
			s_Renderer = nullptr; 
		}
	}
	void renderTexture(const Texture2D& texture, float x, float y)
	{
		SDL_FRect dst{};
		dst.x = x;
		dst.y = y;
		SDL_GetTextureSize(texture.getSDLTexture(), &dst.w, &dst.h);
		SDL_RenderTexture(s_Renderer, texture.getSDLTexture(), nullptr, &dst);
	}
	void renderTexture(const Texture2D& texture, float x, float y, float width, float height)
	{
		SDL_FRect dst{};
		dst.x = x;
		dst.y = y;
		dst.w = width;
		dst.h = height;
		SDL_RenderTexture(s_Renderer, texture.getSDLTexture(), nullptr, &dst);
	}
	void renderTexture(const Texture2D& texture,
		float destinationX, float destinationY, float destinationWidth, float destinationHeight,
		float sourceX, float sourceY, float sourceWidth, float sourceHeight)
	{
		const SDL_FRect source{ sourceX, sourceY, sourceWidth, sourceHeight };
		const SDL_FRect destination{ destinationX, destinationY, destinationWidth, destinationHeight };
		SDL_RenderTexture(s_Renderer, texture.getSDLTexture(), &source, &destination);
	}
	SDL_Renderer* getSDLRenderer()
	{
		return s_Renderer;
	}
}