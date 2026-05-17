#ifndef UNDYNE_APP_H
#define UNDYNE_APP_H

#include <UndyneEngine.h>


class UndyneApp final : public UndyneEngine::Application
{
public:
	UndyneApp() {}
	~UndyneApp() {}

protected:
	void load() override;
};

UndyneEngine::Application* UndyneEngine::createApplication()
{
	return new UndyneApp();
}

#endif
