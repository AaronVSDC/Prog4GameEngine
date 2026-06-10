#include "GameState.h"
#include "Level/LevelLoader.h"
#include "Commands/MoveCommand.h"
#include "Commands/ActionCommand.h"
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
		const std::vector<int> s_LevelSequence{ 1, 2 };
		std::size_t s_LevelCursor{ 0 };

		void buildMenuScene()
		{
			Scene* menu = SceneManager::createScene("Menu");
			if (not menu)
				return;

			int outputWidth = 0;
			int outputHeight = 0;
			Renderer::getOutputSize(outputWidth, outputHeight);
			const float centerX = static_cast<float>(outputWidth) * 0.5f;
			const float height = static_cast<float>(outputHeight);

			auto title = std::make_unique<GameObject>("MenuTitle");
			title->addComponent<TextComponent>("DIGGER", ResourceManager::loadFont("ScoreBoardFont.otf", 72));
			title->getTransform().setLocalPosition(centerX - 130.0f, height * 0.30f, 0.0f);
			menu->add(std::move(title));

			auto prompt = std::make_unique<GameObject>("MenuPrompt");
			prompt->addComponent<TextComponent>("Press SPACE to start", ResourceManager::loadFont("ScoreBoardFont.otf", 32));
			prompt->getTransform().setLocalPosition(centerX - 180.0f, height * 0.55f, 0.0f);
			menu->add(std::move(prompt));
		}

		Scene* prepareLevel(int levelIndex)
		{
			const std::string name = "Level" + std::to_string(levelIndex);
			Scene* scene = SceneManager::getScene(name);
			if (not scene)
				scene = SceneManager::createScene(name);
			else
				scene->removeAll();

			LevelLoader{}.load(levelIndex, *scene);
			return scene;
		}

		void activate(Scene* scene)
		{
			if (not scene)
				return;
			SceneManager::setActiveScene(scene);
			if (not scene->hasStarted())
				scene->start();
		}
	}

	void init()
	{
		InputManager::bindButtonCommand(KeyboardKey::W, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ 0.0f, -1.0f }));
		InputManager::bindButtonCommand(KeyboardKey::S, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ 0.0f,  1.0f }));
		InputManager::bindButtonCommand(KeyboardKey::A, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ -1.0f, 0.0f }));
		InputManager::bindButtonCommand(KeyboardKey::D, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{  1.0f, 0.0f }));
		InputManager::bindButtonCommand(KeyboardKey::Space, InputState::Pressed, std::make_unique<ActionCommand>());

		auto& audio = SoundServiceLocator::getSoundSystem();
		audio.loadSound("Audio/digger.wav", "BackgroundMusic");
		audio.playSound("BackgroundMusic", true);

		buildMenuScene();
		s_Phase = Phase::Menu;
	}

	void startGame()
	{
		if (s_Phase != Phase::Menu)
			return;
		s_LevelCursor = 0;
		activate(prepareLevel(s_LevelSequence[s_LevelCursor]));
		s_Phase = Phase::Playing;
	}

	void winCurrentLevel()
	{
		if (s_Phase != Phase::Playing)
			return;
		++s_LevelCursor;
		if (s_LevelCursor < s_LevelSequence.size())
			activate(prepareLevel(s_LevelSequence[s_LevelCursor]));
		else
			returnToMenu();
	}

	void enterGameOver()
	{
		if (s_Phase == Phase::Playing)
			s_Phase = Phase::GameOver;
	}

	void returnToMenu()
	{
		activate(SceneManager::getScene("Menu"));
		s_Phase = Phase::Menu;
	}

	Phase phase() noexcept { return s_Phase; }
}
