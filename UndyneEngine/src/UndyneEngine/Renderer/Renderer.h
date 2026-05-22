#ifndef RENDERER_H
#define RENDERER_H

#include "..\Core\Core.h"

struct SDL_Window;
struct SDL_Renderer;

namespace UndyneEngine
{
	class Texture2D;
	namespace Renderer
	{
		UNDYNE_API void init(SDL_Window* window); 
		UNDYNE_API void render(); 
		UNDYNE_API void destroy(); 

		UNDYNE_API void renderTexture(const Texture2D& texture, float x, float y); 
		UNDYNE_API void renderTexture(const Texture2D& texture, float x, float y, float width, float height); 

		UNDYNE_API SDL_Renderer* getSDLRenderer(); 
		
	}
}




#endif