#define UNDYNE_ENTRY_POINT
#include <UndyneEngine.h>
#include "UndyneApp.h"
#include "Level/LevelLoader.h"
#include "Commands/MoveCommand.h"
#include "Components/MoveComponent.h"
#include "Components/DigComponent.h"
#include "Components/DigField.h"

#include <glm/glm.hpp>

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

    DigField* digField = levelLoader.getDigField();

    auto player = std::make_unique<GameObject>("Player");

    const int playerColumnCount = 4;
    auto* playerTextureComponent = player->addComponent<TextureComponent>("Sprites/PlayerSprites.png");
    player->addComponent<AnimationComponent>(playerColumnCount);
    // Draw the player centred on its transform so it lines up with the lane it digs.
    playerTextureComponent->setCentered(true);

    if (digField)
    {
        // Size the player a bit smaller than a cell so the round hole shows around it, then
        // drop its centre on the start cell.
        const float frameWidth = playerTextureComponent->getTextureSize().x / playerColumnCount;
        const float playerWidthInCells = 0.7f;
        playerTextureComponent->setScale(digField->cellStride() * playerWidthInCells / frameWidth);

        const glm::ivec2 startCell = levelLoader.getStartCell();
        const glm::vec2 startCenter{ digField->laneCenterX(startCell.x), digField->laneCenterY(startCell.y) };
        player->getTransform().setLocalPosition(startCenter.x, startCenter.y, 0.0f);
    }

    auto* moveComponent = player->addComponent<MoveComponent>();
    moveComponent->setField(digField);

    auto* digComponent = player->addComponent<DigComponent>();
    digComponent->setField(digField);

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