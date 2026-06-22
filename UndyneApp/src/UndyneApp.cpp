#define UNDYNE_ENTRY_POINT
#include <UndyneEngine.h>
#include "UndyneApp.h"

UndyneEngine::Application* UndyneEngine::createApplication()
{
    return new UndyneApp();
}

void UndyneApp::load()
{
}
