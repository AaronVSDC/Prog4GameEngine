#include "Window.h"
#include "..\Log\Log.h"
#include <SDL3\SDL.h>

namespace UndyneEngine
{
    Window::Window(const WindowProps& props)
        : m_Props(props)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            UDE_CORE_ERROR("Failed to initialize SDL3: {}", SDL_GetError());
            return;
        }

        m_Window = SDL_CreateWindow(
            m_Props.title.c_str(),
            m_Props.width,
            m_Props.height,
            SDL_WINDOW_RESIZABLE
        );

        if (!m_Window)
        {
            UDE_CORE_ERROR("Failed to create window: {}", SDL_GetError());
            return;
        }

    }

    Window::~Window()
    {
        if (m_Window)
            SDL_DestroyWindow(m_Window);
        m_Window = nullptr; 
        SDL_Quit();
    }
}