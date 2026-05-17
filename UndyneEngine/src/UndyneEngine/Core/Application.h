#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h" 

//std
#include <memory>
namespace UndyneEngine
{

	class Window; 
	class UNDYNE_ENGINE_API Application
	{

	public: 

		Application();
		virtual ~Application(); 

		Application& operator=(Application& rhs) = delete; 
		Application& operator=(Application&& rhs) = delete;
		Application(Application& other) = delete; 
		Application(Application&& other) = delete; 

		void run(); 
	protected: 
		virtual void load(){}
	private: 
		std::unique_ptr<Window> m_Window;
	};

	//To be defined in client
	Application* createApplication();
}


#endif