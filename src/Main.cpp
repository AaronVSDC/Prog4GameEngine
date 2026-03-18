#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
 

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "Scene.h"
#include <glm/glm.hpp>

#include <filesystem>
#include "MoveCommand.h"
#include "MoveAnalogCommand.h"
#include "InputManager.h"
#include "KillPlayerCommand.h"
#include "HealthComponent.h"
#include "LivesDisplayComponent.h" 
#include "TextComponent.h"
#include "Font.h"
#include "Texture2D.h" 
#include "AddPointsCommand.h"
#include "PointsDisplayComponent.h"
 
namespace fs = std::filesystem;

static void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene();
    auto& input = dae::InputManager::GetInstance();
    input.addController(0);
    input.addController(1);

    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

    auto player1 = std::make_unique<dae::GameObject>();
    player1->AddComponent(std::make_unique<dae::TextureComponent>("logo.png"));
    player1->SetLocalPosition(glm::vec3(200, 200, 0));
    player1->AddComponent(std::make_unique<dae::HealthComponent>(3, 1));
    player1->AddComponent(std::make_unique<dae::ScoreComponent>());
    auto* health1 = player1->GetComponent<dae::HealthComponent>();
    auto* score1 = player1->GetComponent<dae::ScoreComponent>();

    auto player2 = std::make_unique<dae::GameObject>();
    player2->AddComponent(std::make_unique<dae::TextureComponent>("logo.png"));
    player2->SetLocalPosition(glm::vec3(400, 200, 0));
    player2->AddComponent(std::make_unique<dae::HealthComponent>(3, 1));
    player2->AddComponent(std::make_unique<dae::ScoreComponent>());
    auto* health2 = player2->GetComponent<dae::HealthComponent>();
    auto* score2 = player2->GetComponent<dae::ScoreComponent>();

    auto p1Lives = std::make_unique<dae::GameObject>(); 
    p1Lives->SetLocalPosition(glm::vec3(10, 10, 0));
    p1Lives->AddComponent(std::make_unique<dae::TextComponent>("P1 Lives: 3", font));
    p1Lives->AddComponent(std::make_unique<dae::LivesDisplayComponent>(3));
    health1->AddObserver(p1Lives->GetComponent<dae::LivesDisplayComponent>());

    auto p1Points = std::make_unique<dae::GameObject>();
    p1Points->SetLocalPosition(glm::vec3(10, 35, 0));
    p1Points->AddComponent(std::make_unique<dae::TextComponent>("P1 Score: 0", font));
    p1Points->AddComponent(std::make_unique<dae::PointsDisplayComponent>());
    score1->AddObserver(p1Points->GetComponent<dae::PointsDisplayComponent>());

    auto p2Lives = std::make_unique<dae::GameObject>();
    p2Lives->SetLocalPosition(glm::vec3(10, 60,  0));
    p2Lives->AddComponent(std::make_unique<dae::TextComponent>("P2 Lives: 3", font));
    p2Lives->AddComponent(std::make_unique<dae::LivesDisplayComponent>(3));
    health2->AddObserver(p2Lives->GetComponent<dae::LivesDisplayComponent>());

    auto p2Points = std::make_unique<dae::GameObject>();
    p2Points->SetLocalPosition(glm::vec3(10, 85, 0));
    p2Points->AddComponent(std::make_unique<dae::TextComponent>("P2 Score: 0", font));
    p2Points->AddComponent(std::make_unique<dae::PointsDisplayComponent>());
    score2->AddObserver(p2Points->GetComponent<dae::PointsDisplayComponent>());

    input.addKeyboardCommand(SDL_SCANCODE_W, dae::KeyState::Down,
        std::make_unique<dae::MoveCommand>(player1.get(), glm::vec2(0.f, -1.f), 2.f));
    input.addKeyboardCommand(SDL_SCANCODE_S, dae::KeyState::Down,
        std::make_unique<dae::MoveCommand>(player1.get(), glm::vec2(0.f, 1.f), 2.f));
    input.addKeyboardCommand(SDL_SCANCODE_A, dae::KeyState::Down,
        std::make_unique<dae::MoveCommand>(player1.get(), glm::vec2(-1.f, 0.f), 2.f));
    input.addKeyboardCommand(SDL_SCANCODE_D, dae::KeyState::Down,
        std::make_unique<dae::MoveCommand>(player1.get(), glm::vec2(1.f, 0.f), 2.f));
    input.addKeyboardCommand(SDL_SCANCODE_K, dae::KeyState::Pressed,
        std::make_unique<dae::KillPlayerCommand>(health1));
    input.addKeyboardCommand(SDL_SCANCODE_E, dae::KeyState::Pressed,
        std::make_unique<dae::AddPointsCommand>(score1, 100, dae::Event::ENEMY_DIED));
    input.addKeyboardCommand(SDL_SCANCODE_P, dae::KeyState::Pressed,
        std::make_unique<dae::AddPointsCommand>(score1, 50, dae::Event::PICKUP_COLLECTED));

    input.addControllerStickCommand(0, dae::Stick::Left, 0.20f, true,
        std::make_unique<dae::MoveAnalogCommand>(player2.get(), 70.f));
    input.addControllerCommand(0, dae::ControllerButton::DPadUp, dae::KeyState::Down,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec2(0.f, -1.f), 2.f));
    input.addControllerCommand(0, dae::ControllerButton::DPadDown, dae::KeyState::Down,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec2(0.f, 1.f), 2.f));
    input.addControllerCommand(0, dae::ControllerButton::DPadLeft, dae::KeyState::Down,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec2(-1.f, 0.f), 2.f));
    input.addControllerCommand(0, dae::ControllerButton::DPadRight, dae::KeyState::Down,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec2(1.f, 0.f), 2.f));
    input.addControllerCommand(0, dae::ControllerButton::A, dae::KeyState::Pressed,
        std::make_unique<dae::KillPlayerCommand>(health2));
    input.addControllerCommand(0, dae::ControllerButton::X, dae::KeyState::Pressed,
        std::make_unique<dae::AddPointsCommand>(score2, 100, dae::Event::ENEMY_DIED));
    input.addControllerCommand(0, dae::ControllerButton::Y, dae::KeyState::Pressed,
        std::make_unique<dae::AddPointsCommand>(score2, 50, dae::Event::PICKUP_COLLECTED));
    auto controls1 = std::make_unique<dae::GameObject>();
    controls1->SetLocalPosition(glm::vec3(10, 430, 0));
    controls1->AddComponent(std::make_unique<dae::TextComponent>("P1: WASD = Move  |  K = Lose a life  |  E = Enemy killed (+100)  |  P = Pickup (+50)", font));

    auto controls2 = std::make_unique<dae::GameObject>();
    controls2->SetLocalPosition(glm::vec3(10, 455, 0));
    controls2->AddComponent(std::make_unique<dae::TextComponent>("P2: DPad/LStick = Move  |  A = Lose a life  |  X = Enemy killed (+100)  |  Y = Pickup (+50)", font));

    auto controls3 = std::make_unique<dae::GameObject>();
    controls3->SetLocalPosition(glm::vec3(10, 480, 0));
    controls3->AddComponent(std::make_unique<dae::TextComponent>("Reach 500 points to unlock the Steam achievement: ACH_WIN_ONE_GAME", font));

    scene.Add(std::move(player1));
    scene.Add(std::move(player2));
    scene.Add(std::move(p1Lives));
    scene.Add(std::move(p1Points));
    scene.Add(std::move(p2Lives));
    scene.Add(std::move(p2Points));
    scene.Add(std::move(controls1));
    scene.Add(std::move(controls2));
    scene.Add(std::move(controls3));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/"; 
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
