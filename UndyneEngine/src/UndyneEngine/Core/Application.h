#ifndef APPLICATION_H
#define APPLICATION_H


//std
#include <memory>
namespace UndyneEngine
{

	class Window; 
	class Application
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