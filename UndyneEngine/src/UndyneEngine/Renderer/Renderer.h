#ifndef RENDERER_H
#define RENDERER_H

struct SDL_Window;
struct SDL_Renderer;

namespace UndyneEngine
{
	struct Texture2D;
	namespace Renderer
	{
		void init(SDL_Window* window); 
		void render(); 
		void destroy(); 

		void renderTexture(const Texture2D& texture, float x, float y); 
		void renderTexture(const Texture2D& texture, float x, float y, float width, float height); 
		
		SDL_Renderer* getSDLRenderer(); 
		
	}
}




#endif