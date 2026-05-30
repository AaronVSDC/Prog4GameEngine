#define UNDYNE_ENTRY_POINT
#include <UndyneEngine.h>
#include "UndyneApp.h"
#include "Components/RotatorComponent.h"
#include "Commands/MoveCommand.h"

using namespace UndyneEngine; 
using namespace Digger; 

UndyneEngine::Application* UndyneEngine::createApplication()
{
    return new UndyneApp();
}

void UndyneApp::load()
{ 
    auto* scene = SceneManager::createScene("DemoScene");

    auto* sun = scene->createGameObject("sun");
    sun->getTransform().setLocalPosition(400.0f, 300.0f, 0.0f);
    sun->addComponent<TextureComponent>("logo.png");   

    auto* earth = scene->createGameObject("earth");
    earth->setParent(sun);                               
    earth->addComponent<TextureComponent>("logo.png");
    earth->addComponent<RotatorComponent>(120.0f, 1.0f);   

    auto* moon = scene->createGameObject("moon");
    moon->addComponent<TextureComponent>("logo.png");
    
    auto keyboard = InputManager::addController(ControllerType::Keyboard); 


    InputManager::bindCommand<MoveCommand>(keyboard, KeyboardKey::W, InputState::Down, moon, 0.f, -2.f); 

    auto& audio = SoundServiceLocator::getSoundSystem();

    audio.loadSound("audio/digger.wav", "BackgroundMusic"); 
    audio.playSound("BackgroundMusic", true);
}