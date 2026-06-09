#define UNDYNE_ENTRY_POINT
#include <UndyneEngine.h>
#include "UndyneApp.h"
#include "Level/LevelLoader.h"

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
    auto* scene = SceneManager::createScene("Level1");
    LevelLoader{}.load(1, *scene);
}
