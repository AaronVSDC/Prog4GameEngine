#define UNDYNE_ENTRY_POINT
#include <UndyneEngine.h>
#include "UndyneApp.h"
#include "Level/LevelLoader.h"
#include "Commands/MoveCommand.h"

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

    LevelLoader levelLoader;
    levelLoader.loadLevel(0, *scene);

    auto& audio = SoundServiceLocator::getSoundSystem();
    audio.loadSound("Audio/digger.wav", "BackgroundMusic");
    audio.playSound("BackgroundMusic", true);
}