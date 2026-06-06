#ifndef RENDERER_H
#define RENDERER_H

#include "../Core/Core.h"

#include <glm/vec2.hpp>
#include <cstdint>

struct SDL_Window;
struct SDL_Renderer;

namespace UndyneEngine
{
	class Texture2D;
	namespace Renderer
	{
		enum class Flip : std::uint8_t { None, Horizontal, Vertical };

		struct Rect
		{
			glm::vec2 position{ 0.0f, 0.0f };
			glm::vec2 size{ 0.0f, 0.0f };
		};

		struct TextureRenderInfo
		{
			Rect source{};
			Rect destination{};
			Flip flip{ Flip::None };
			float rotation{ 0.0f };
		};

		UNDYNE_API void init(SDL_Window* window);
		UNDYNE_API void render(); 
		UNDYNE_API void destroy(); 

		UNDYNE_API void renderTexture(const Texture2D& texture, float x, float y); 
		UNDYNE_API void renderTexture(const Texture2D& texture, float x, float y, float width, float height); 
		UNDYNE_API void renderTexture(const Texture2D& texture, const TextureRenderInfo& renderInfo);
		UNDYNE_API SDL_Renderer* getSDLRenderer(); 
		
	}
}




#endif