#ifndef RENDERER_H
#define RENDERER_H


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
		enum class Flip : uint8_t { None, Horizontal, Vertical };

		struct Color 
		{
			uint8_t r{ 255 }; 
			uint8_t g{ 255 }; 
			uint8_t b{ 255 }; 
			uint8_t a{ 255 };
		};

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

		void init(SDL_Window* window);
		void render(); 
		void destroy(); 

		void renderTexture(const Texture2D& texture, float x, float y); 
		void renderTexture(const Texture2D& texture, float x, float y, float width, float height); 
		void renderTexture(const Texture2D& texture, const TextureRenderInfo& renderInfo);
		SDL_Renderer* getSDLRenderer();
		void getOutputSize(int& width, int& height);
		std::unique_ptr<Texture2D> createRenderTarget(int width, int height);
		void setRenderTarget(Texture2D* target);
		std::unique_ptr<Texture2D> createTextTexture(const Font& font, const std::string& text, Color color);

	}
}




#endif