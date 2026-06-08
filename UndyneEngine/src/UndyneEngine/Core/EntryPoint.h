#ifndef ENTRY_POINT_H 
#define ENTRY_POINT_H
#include <UndyneEngine.h>

#ifdef UDE_PLATFORM_WINDOWS 


extern UndyneEngine::Application* UndyneEngine::createApplication();  

int main(int, char* [])
{
	auto app = UndyneEngine::createApplication();
	app->run();
	delete app;
}


#endif
#endif

