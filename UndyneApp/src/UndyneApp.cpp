#include <UndyneEngine.h>


class UndyneApp final : public UndyneEngine::Application
{
public:
	UndyneApp() {}
	~UndyneApp() {}

};

UndyneEngine::Application* UndyneEngine::createApplication()
{
	return new UndyneApp(); 
}


