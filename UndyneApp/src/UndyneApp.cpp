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


    auto player = std::make_unique<GameObject>("Player"); 

    auto playerTextureComponent = player->addComponent<TextureComponent>("Sprites/PlayerSprites.png"); 
    playerTextureComponent->setScale(1.7f); 
    int playerColumnCount = 4; 
    player->addComponent<AnimationComponent>(playerColumnCount); 
    player->addComponent<MoveComponent>(); 

    //commands
    //---------
    InputManager::bindButtonCommand(
        KeyboardKey::W,
        InputState::Down,
        std::make_unique<MoveCommand>(player.get(), glm::vec2{ 0.f, -1.f })
    );
    InputManager::bindButtonCommand(
        KeyboardKey::S,
        InputState::Down,
        std::make_unique<MoveCommand>(player.get(), glm::vec2{ 0.f, 1.f })
    );
    InputManager::bindButtonCommand(
        KeyboardKey::A,
        InputState::Down,
        std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1.f, 0.f })
    );
    InputManager::bindButtonCommand(
        KeyboardKey::D,
        InputState::Down,
        std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1.f, 0.f })
    );



    //audio
    //------
    auto& audio = SoundServiceLocator::getSoundSystem();
    audio.loadSound("Audio/digger.wav", "BackgroundMusic");
    audio.playSound("BackgroundMusic", true);

    scene->add(std::move(player));
}