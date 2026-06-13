#include "GameState.h"
#include "../Level/LevelLoader.h"
#include "../Commands/MoveCommand.h"
#include "../Commands/ActionCommand.h"
#include "../Commands/StartModeCommand.h"
#include "../Commands/SecondPlayerMoveCommand.h"
#include "../Commands/FireCommand.h"
#include <UndyneEngine.h>

//std
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

using namespace UndyneEngine;

namespace Digger::GameState
{
	namespace
	{
		Phase s_Phase{ Phase::Menu };
		Mode s_Mode{ Mode::Normal };
		bool s_ActionPaused{ false };
		const std::vector<int> s_LevelSequence{ 0, 1, 2 };
		std::size_t s_LevelCursor{ 0 };

		void buildMenuScene()
		{
			Scene* menu = SceneManager::createScene("Menu");
			if (!menu)
				return;

			int outputWidth = 0;
			int outputHeight = 0;
			Renderer::getOutputSize(outputWidth, outputHeight);
			const float centerX = static_cast<float>(outputWidth) * 0.5f;
			const float height = static_cast<float>(outputHeight);

			auto title = std::make_unique<GameObject>("MenuTitle");
			title->addComponent<TextComponent>("DIGGER", ResourceManager::loadFont("ScoreBoardFont.otf", 72))->setCentered(true);
			title->getTransform().setLocalPosition(centerX, height * 0.25f, 0.0f);
			menu->add(std::move(title));

			auto prompt = std::make_unique<GameObject>("MenuPrompt");
			prompt->addComponent<TextComponent>("1: NORMAL   2: CO-OP   3: VERSUS", ResourceManager::loadFont("ScoreBoardFont.otf", 28))->setCentered(true);
			prompt->getTransform().setLocalPosition(centerX, height * 0.55f, 0.0f);
			menu->add(std::move(prompt));
		}

		Scene* prepareLevel(int levelIndex)
		{
			const std::string name = "Level" + std::to_string(levelIndex);
			Scene* scene = SceneManager::getScene(name);
			if (!scene)
				scene = SceneManager::createScene(name);
			else
				scene->removeAll();

			LevelLoader{}.load(levelIndex, *scene, s_Mode);
			return scene;
		}

		void activate(Scene* scene)
		{
			if (!scene)
				return;
			SceneManager::setActiveScene(scene);
			if (!scene->hasStarted())
				scene->start();
		}

		void bindInput()
		{
			InputManager::bindButtonCommand(KeyboardKey::W, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ 0.0f, -1.0f }));
			InputManager::bindButtonCommand(KeyboardKey::S, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ 0.0f,  1.0f }));
			InputManager::bindButtonCommand(KeyboardKey::A, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ -1.0f, 0.0f }));
			InputManager::bindButtonCommand(KeyboardKey::D, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{  1.0f, 0.0f }));
			InputManager::bindButtonCommand(KeyboardKey::Space, InputState::Pressed, std::make_unique<ActionCommand>());

			InputManager::bindButtonCommand(KeyboardKey::Num1, InputState::Pressed, std::make_unique<StartModeCommand>(Mode::Normal));
			InputManager::bindButtonCommand(KeyboardKey::Num2, InputState::Pressed, std::make_unique<StartModeCommand>(Mode::Coop));
			InputManager::bindButtonCommand(KeyboardKey::Num3, InputState::Pressed, std::make_unique<StartModeCommand>(Mode::Versus));

			const ControllerID controller = InputManager::addController();
			InputManager::bindButtonCommand(controller, GamepadButton::DPadUp, InputState::Down, std::make_unique<SecondPlayerMoveCommand>(glm::vec2{ 0.0f, -1.0f }));
			InputManager::bindButtonCommand(controller, GamepadButton::DPadDown, InputState::Down, std::make_unique<SecondPlayerMoveCommand>(glm::vec2{ 0.0f,  1.0f }));
			InputManager::bindButtonCommand(controller, GamepadButton::DPadLeft, InputState::Down, std::make_unique<SecondPlayerMoveCommand>(glm::vec2{ -1.0f, 0.0f }));
			InputManager::bindButtonCommand(controller, GamepadButton::DPadRight, InputState::Down, std::make_unique<SecondPlayerMoveCommand>(glm::vec2{  1.0f, 0.0f }));
			InputManager::bindButtonCommand(controller, GamepadButton::A, InputState::Pressed, std::make_unique<FireCommand>("Player2"));
		}
	}

	void init()
	{
		bindInput();
		buildMenuScene();
		s_Phase = Phase::Menu;
		SoundServiceLocator::getSoundSystem().loadSound("Audio/digger.wav", "digger"); 
	}

	void startGame(Mode mode)
	{
		if (s_Phase != Phase::Menu)
			return;
		s_Mode = mode;
		s_LevelCursor = 0;
		s_ActionPaused = false;
		activate(prepareLevel(s_LevelSequence[s_LevelCursor]));
		SoundServiceLocator::getSoundSystem().playSound("digger", true); 
		s_Phase = Phase::Playing;
	}

	void winCurrentLevel()
	{
		if (s_Phase != Phase::Playing)
			return;
		++s_LevelCursor;
		s_ActionPaused = false;
		if (s_LevelCursor < s_LevelSequence.size())
			activate(prepareLevel(s_LevelSequence[s_LevelCursor]));
		else
			returnToMenu();
	}

	void enterGameOver()
	{
		if (s_Phase == Phase::Playing)
		{
			s_Phase = Phase::GameOver;
			SoundServiceLocator::getSoundSystem().stopSound("digger"); 
		}
	}

	void returnToMenu()
	{
		s_ActionPaused = false;
		activate(SceneManager::getScene("Menu"));
		s_Phase = Phase::Menu;
		SoundServiceLocator::getSoundSystem().stopSound("digger"); 
	}

	Phase phase() noexcept { return s_Phase; }
	Mode mode() noexcept { return s_Mode; }

	void pauseAction() noexcept { s_ActionPaused = true; }
	void resumeAction() noexcept { s_ActionPaused = false; }
	bool isActionPaused() noexcept { return s_ActionPaused; }
}
