#ifndef ENTRY_POINT_H 
#define ENTRY_POINT_H
#include <UndyneEngine.h>

extern UndyneEngine::Application* UndyneEngine::createApplication();  

int main(int, char* [])
{
	auto app = UndyneEngine::createApplication();
	app->run();
	delete app;
}


#endif

