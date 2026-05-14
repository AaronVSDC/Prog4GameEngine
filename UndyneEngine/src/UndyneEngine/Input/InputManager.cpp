#include "InputManager.h"
#include <SDL3/SDL.h>

namespace UndyneEngine::InputManager
{

	bool processInput()
	{
		SDL_Event e; 

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_EVENT_QUIT)
			{
				return false; 
			}
		}

		return true; 
	}

}
