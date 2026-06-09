#ifndef RENDERER_H
#define RENDERER_H

#include "../Core/Core.h"

#include <glm/vec2.hpp>
#include <cstdint>
#include <memory>
#include <string>

struct SDL_Window;
struct SDL_Renderer;

namespace UndyneEngine
{
	class Texture2D;
	class Font;
	namespace Renderer
	{
		enum class Flip : std::uint8_t { None, Horizontal, Vertical };

		struct Color { std::uint8_t r{ 255 }, g{ 255 }, b{ 255 }, a{ 255 }; };

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
		UNDYNE_API void getOutputSize(int& width, int& height);
		UNDYNE_API std::unique_ptr<Texture2D> createRenderTarget(int width, int height);
		UNDYNE_API void setRenderTarget(Texture2D* target);
		UNDYNE_API std::unique_ptr<Texture2D> createTextTexture(const Font& font, const std::string& text, Color color);

	}
}




#endif