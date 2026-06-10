#define UNDYNE_ENTRY_POINT
#include <UndyneEngine.h>
#include "UndyneApp.h"
#include "GameState.h"

#ifdef UDE_DEBUG
#include <vld.h>
#endif


using namespace UndyneEngine;
using namespace Digger;

UndyneEngine::Application* UndyneEngine::createApplication()
{
    return new UndyneApp();
}

void UndyneApp::load()
{
    GameState::init();
}
