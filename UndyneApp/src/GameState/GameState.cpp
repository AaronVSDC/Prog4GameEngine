#include "GameState.h"
#include "../Level/LevelLoader.h"
#include "../Commands/MoveCommand.h"
#include "../Commands/ActionCommand.h"
#include "../Commands/StartModeCommand.h"
#include "../Commands/SecondPlayerMoveCommand.h"
#include "../Commands/FireCommand.h"
#include "../Commands/HighScoreCommands.h"
#include "../Commands/SystemCommands.h"
#include "../Commands/GamepadFireCommand.h"
#include "../Components/ScoreComponent.h"
#include "../Components/NameEntryComponent.h"
#include "../HighScore/HighScoreTable.h"
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
		int s_RunScore{ 0 };
		int s_FinalScore{ 0 };

		int currentPlayerScore()
		{
			if (Scene* scene = SceneManager::getActiveScene())
				if (GameObject* player = scene->findGameObjectByName("Player"))
					if (ScoreComponent* score = player->getComponent<ScoreComponent>())
						return score->getScore();
			return 0;
		}

		void addHighScoreList(Scene& scene, float centerX, float height, float topY)
		{
			Font* headerFont = ResourceManager::loadFont("ScoreBoardFont.otf", 26);
			Font* rowFont = ResourceManager::loadFont("ScoreBoardFont.otf", 22);

			auto header = std::make_unique<GameObject>("HighScoreHeader");
			header->addComponent<TextComponent>("HIGH SCORES", headerFont)->setCentered(true);
			header->getTransform().setLocalPosition(centerX, topY, 0.0f);
			scene.add(std::move(header));

			const float rowSpacing = height * 0.045f;
			float rowY = topY + height * 0.06f;
			int rank = 1;
			for (const HighScores::Entry& entry : HighScores::entries())
			{
				const std::string line = std::to_string(rank) + ".   " + entry.initials + "     " + std::to_string(entry.score);
				auto row = std::make_unique<GameObject>("HighScoreRow" + std::to_string(rank));
				row->addComponent<TextComponent>(line, rowFont)->setCentered(true);
				row->getTransform().setLocalPosition(centerX, rowY, 0.0f);
				scene.add(std::move(row));
				rowY += rowSpacing;
				++rank;
			}
		}

		Scene* buildMenuScene()
		{
			Scene* menu = SceneManager::getScene("Menu");
			if (!menu)
				menu = SceneManager::createScene("Menu");
			else
				menu->removeAll();

			int outputWidth = 0;
			int outputHeight = 0;
			Renderer::getOutputSize(outputWidth, outputHeight);
			const float centerX = static_cast<float>(outputWidth) * 0.5f;
			const float height = static_cast<float>(outputHeight);

			auto title = std::make_unique<GameObject>("MenuTitle");
			title->addComponent<TextComponent>("DIGGER", ResourceManager::loadFont("ScoreBoardFont.otf", 72))->setCentered(true);
			title->getTransform().setLocalPosition(centerX, height * 0.12f, 0.0f);
			menu->add(std::move(title));

			auto prompt = std::make_unique<GameObject>("MenuPrompt");
			prompt->addComponent<TextComponent>("1 / A: NORMAL     2 / X: CO-OP     3 / Y: VERSUS", ResourceManager::loadFont("ScoreBoardFont.otf", 28))->setCentered(true);
			prompt->getTransform().setLocalPosition(centerX, height * 0.24f, 0.0f);
			menu->add(std::move(prompt));

			addHighScoreList(*menu, centerX, height, height * 0.34f);
			return menu;
		}

		Scene* buildEndScene(bool qualifies, int finalScore)
		{
			Scene* endScene = SceneManager::getScene("GameOver");
			if (!endScene)
				endScene = SceneManager::createScene("GameOver");
			else
				endScene->removeAll();

			int outputWidth = 0;
			int outputHeight = 0;
			Renderer::getOutputSize(outputWidth, outputHeight);
			const float centerX = static_cast<float>(outputWidth) * 0.5f;
			const float height = static_cast<float>(outputHeight);

			auto title = std::make_unique<GameObject>("GameOverTitle");
			title->addComponent<TextComponent>("GAME OVER", ResourceManager::loadFont("ScoreBoardFont.otf", 48))->setCentered(true);
			title->getTransform().setLocalPosition(centerX, height * 0.08f, 0.0f);
			endScene->add(std::move(title));

			auto scoreText = std::make_unique<GameObject>("GameOverScore");
			scoreText->addComponent<TextComponent>("YOUR SCORE   " + std::to_string(finalScore), ResourceManager::loadFont("ScoreBoardFont.otf", 30))->setCentered(true);
			scoreText->getTransform().setLocalPosition(centerX, height * 0.16f, 0.0f);
			endScene->add(std::move(scoreText));

			if (qualifies)
			{
				auto label = std::make_unique<GameObject>("NewHighScoreLabel");
				label->addComponent<TextComponent>("NEW HIGH SCORE!   ENTER YOUR INITIALS", ResourceManager::loadFont("ScoreBoardFont.otf", 24))->setCentered(true);
				label->getTransform().setLocalPosition(centerX, height * 0.24f, 0.0f);
				endScene->add(std::move(label));

				auto entry = std::make_unique<GameObject>("NameEntry");
				entry->addComponent<TextComponent>("[A] A  A", ResourceManager::loadFont("ScoreBoardFont.otf", 44))->setCentered(true);
				entry->addComponent<NameEntryComponent>();
				entry->getTransform().setLocalPosition(centerX, height * 0.31f, 0.0f);
				endScene->add(std::move(entry));

				auto hint = std::make_unique<GameObject>("NameEntryHint");
				hint->addComponent<TextComponent>("UP / DOWN: LETTER     LEFT / RIGHT: MOVE     ENTER or START: OK", ResourceManager::loadFont("ScoreBoardFont.otf", 18))->setCentered(true);
				hint->getTransform().setLocalPosition(centerX, height * 0.37f, 0.0f);
				endScene->add(std::move(hint));
			}
			else
			{
				auto prompt = std::make_unique<GameObject>("ContinuePrompt");
				prompt->addComponent<TextComponent>("PRESS SPACE / ENTER or START", ResourceManager::loadFont("ScoreBoardFont.otf", 24))->setCentered(true);
				prompt->getTransform().setLocalPosition(centerX, height * 0.27f, 0.0f);
				endScene->add(std::move(prompt));
			}

			addHighScoreList(*endScene, centerX, height, height * 0.45f);
			return endScene;
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

		void finishGame(int finalScore)
		{
			s_FinalScore = finalScore;
			s_ActionPaused = false;
			SoundServiceLocator::getSoundSystem().stopSound("digger");

			const bool qualifies = HighScores::qualifies(finalScore);
			s_Phase = qualifies ? Phase::EnterHighScore : Phase::GameOver;
			activate(buildEndScene(qualifies, finalScore));
		}

		void advanceLevel()
		{
			s_RunScore += currentPlayerScore();
			++s_LevelCursor;
			s_ActionPaused = false;
			if (s_LevelCursor < s_LevelSequence.size())
				activate(prepareLevel(s_LevelSequence[s_LevelCursor]));
			else
				finishGame(s_RunScore);
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

			InputManager::bindButtonCommand(KeyboardKey::F1, InputState::Pressed, std::make_unique<SkipLevelCommand>());
			InputManager::bindButtonCommand(KeyboardKey::F2, InputState::Pressed, std::make_unique<MuteToggleCommand>());

			InputManager::bindButtonCommand(KeyboardKey::Up,    InputState::Pressed, std::make_unique<HighScoreLetterCommand>(1));
			InputManager::bindButtonCommand(KeyboardKey::Down,  InputState::Pressed, std::make_unique<HighScoreLetterCommand>(-1));
			InputManager::bindButtonCommand(KeyboardKey::Left,  InputState::Pressed, std::make_unique<HighScoreCursorCommand>(-1));
			InputManager::bindButtonCommand(KeyboardKey::Right, InputState::Pressed, std::make_unique<HighScoreCursorCommand>(1));
			InputManager::bindButtonCommand(KeyboardKey::Enter, InputState::Pressed, std::make_unique<HighScoreConfirmCommand>());

			const ControllerID controllerOne = InputManager::addController();
			InputManager::bindButtonCommand(controllerOne, GamepadButton::DPadUp, InputState::Down, std::make_unique<SecondPlayerMoveCommand>(glm::vec2{ 0.0f, -1.0f }));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::DPadDown, InputState::Down, std::make_unique<SecondPlayerMoveCommand>(glm::vec2{ 0.0f,  1.0f }));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::DPadLeft, InputState::Down, std::make_unique<SecondPlayerMoveCommand>(glm::vec2{ -1.0f, 0.0f }));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::DPadRight, InputState::Down, std::make_unique<SecondPlayerMoveCommand>(glm::vec2{  1.0f, 0.0f }));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::A, InputState::Pressed, std::make_unique<GamepadFireCommand>());

			InputManager::bindButtonCommand(controllerOne, GamepadButton::A, InputState::Pressed, std::make_unique<StartModeCommand>(Mode::Normal));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::X, InputState::Pressed, std::make_unique<StartModeCommand>(Mode::Coop));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::Y, InputState::Pressed, std::make_unique<StartModeCommand>(Mode::Versus));

			InputManager::bindButtonCommand(controllerOne, GamepadButton::DPadUp,    InputState::Pressed, std::make_unique<HighScoreLetterCommand>(1));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::DPadDown,  InputState::Pressed, std::make_unique<HighScoreLetterCommand>(-1));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::DPadLeft,  InputState::Pressed, std::make_unique<HighScoreCursorCommand>(-1));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::DPadRight, InputState::Pressed, std::make_unique<HighScoreCursorCommand>(1));
			InputManager::bindButtonCommand(controllerOne, GamepadButton::Start,     InputState::Pressed, std::make_unique<HighScoreConfirmCommand>());

			const ControllerID controllerTwo = InputManager::addController();
			InputManager::bindButtonCommand(controllerTwo, GamepadButton::DPadUp, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ 0.0f, -1.0f }));
			InputManager::bindButtonCommand(controllerTwo, GamepadButton::DPadDown, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ 0.0f,  1.0f }));
			InputManager::bindButtonCommand(controllerTwo, GamepadButton::DPadLeft, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{ -1.0f, 0.0f }));
			InputManager::bindButtonCommand(controllerTwo, GamepadButton::DPadRight, InputState::Down, std::make_unique<MoveCommand>(glm::vec2{  1.0f, 0.0f }));
			InputManager::bindButtonCommand(controllerTwo, GamepadButton::A, InputState::Pressed, std::make_unique<FireCommand>("Player"));
		}
	}

	void init()
	{
		HighScores::load();
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
		s_RunScore = 0;
		s_ActionPaused = false;
		activate(prepareLevel(s_LevelSequence[s_LevelCursor]));
		SoundServiceLocator::getSoundSystem().playSound("digger", true);
		s_Phase = Phase::Playing;
	}

	void winCurrentLevel()
	{
		if (s_Phase != Phase::Playing)
			return;
		advanceLevel();
	}

	void skipLevel()
	{
		if (s_Phase != Phase::Playing)
			return;
		advanceLevel();
	}

	void enterGameOver()
	{
		if (s_Phase != Phase::Playing)
			return;
		finishGame(s_RunScore + currentPlayerScore());
	}

	void returnToMenu()
	{
		s_ActionPaused = false;
		s_Phase = Phase::Menu;
		activate(buildMenuScene());
		SoundServiceLocator::getSoundSystem().stopSound("digger");
	}

	void submitHighScore(const std::string& initials)
	{
		if (s_Phase != Phase::EnterHighScore)
			return;
		HighScores::insert(initials, s_FinalScore);
		HighScores::save();
		returnToMenu();
	}

	Phase phase() noexcept { return s_Phase; }
	Mode mode() noexcept { return s_Mode; }

	void pauseAction() noexcept { s_ActionPaused = true; }
	void resumeAction() noexcept { s_ActionPaused = false; }
	bool isActionPaused() noexcept { return s_ActionPaused; }
}
