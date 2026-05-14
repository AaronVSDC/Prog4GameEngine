#ifndef WINDOW_H
#define WINDOW_H

#include "Core.h"

//std
#include <string>


struct SDL_Window; 
namespace UndyneEngine
{
	struct UNDYNE_ENGINE_API WindowProps
	{
		std::string title; 
		int width; 
		int height; 

		WindowProps(const std::string& title = "Undyne Engine",
			int width = 1280,
			int height = 720)
			: title(title), width(width), height(height) {}
	};


    class UNDYNE_ENGINE_API Window final
    {
    public:
        Window(const WindowProps& props = WindowProps{});
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        //void pollEvents();

        int  getWidth()  const { return m_Props.width; }
        int  getHeight() const { return m_Props.height; }
        bool shouldClose() const { return m_ShouldClose; }

    private:
        SDL_Window*  m_Window{ nullptr };
        WindowProps  m_Props;
        bool         m_ShouldClose{ false };
    };

}


#endif

