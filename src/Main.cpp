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

namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto daeLogo = std::make_unique<dae::GameObject>();
	daeLogo->AddComponent(std::make_unique<dae::TextureComponent>("logo.png")); 
	daeLogo->SetLocalPosition(glm::vec3(200, 200,0));

	auto& input = dae::InputManager::GetInstance(); 
	input.addController(0);
	input.addControllerStickCommand(0, dae::Stick::Left, 0.20f, true, std::make_unique<dae::MoveAnalogCommand>(daeLogo.get(), 70.f));

	input.addKeyboardCommand(SDL_SCANCODE_W, dae::KeyState::Down,
		std::make_unique<dae::MoveCommand>(daeLogo.get(), glm::vec2(0.f, -1.f), 2.f));

	input.addKeyboardCommand(SDL_SCANCODE_S, dae::KeyState::Down,
		std::make_unique<dae::MoveCommand>(daeLogo.get(), glm::vec2(0.f, 1.f), 2.f));

	input.addKeyboardCommand(SDL_SCANCODE_A, dae::KeyState::Down,
		std::make_unique<dae::MoveCommand>(daeLogo.get(), glm::vec2(-1.f, 0.f), 2.f));

	input.addKeyboardCommand(SDL_SCANCODE_D, dae::KeyState::Down,
		std::make_unique<dae::MoveCommand>(daeLogo.get(), glm::vec2(1.f, 0.f), 2.f));
	
	input.addControllerCommand(0, dae::ControllerButton::DPadUp, dae::KeyState::Down,
		std::make_unique<dae::MoveCommand>(daeLogo.get(), glm::vec2(0.f, -1.f), 2.f));

	input.addControllerCommand(0, dae::ControllerButton::DPadDown, dae::KeyState::Down,
		std::make_unique<dae::MoveCommand>(daeLogo.get(), glm::vec2(0.f, 1.f), 2.f));

	input.addControllerCommand(0, dae::ControllerButton::DPadLeft, dae::KeyState::Down,
		std::make_unique<dae::MoveCommand>(daeLogo.get(), glm::vec2(-1.f, 0.f), 2.f));

	input.addControllerCommand(0, dae::ControllerButton::DPadRight, dae::KeyState::Down,
		std::make_unique<dae::MoveCommand>(daeLogo.get(), glm::vec2(1.f, 0.f), 2.f));
	
	
	scene.Add(std::move(daeLogo));

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
