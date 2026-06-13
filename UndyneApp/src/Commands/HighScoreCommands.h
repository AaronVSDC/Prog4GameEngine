#ifndef HIGH_SCORE_COMMANDS_H
#define HIGH_SCORE_COMMANDS_H
#include <UndyneEngine.h>
#include "../GameState/GameState.h"
#include "../Components/NameEntryComponent.h"

namespace Digger
{
	class HighScoreLetterCommand final : public UndyneEngine::Command
	{
	public:
		explicit HighScoreLetterCommand(int delta) noexcept
			: m_Delta{ delta }
		{
		}

		void execute() override
		{
			if (GameState::phase() != GameState::Phase::EnterHighScore)
				return;
			if (UndyneEngine::Scene* scene = UndyneEngine::SceneManager::getActiveScene())
				if (UndyneEngine::GameObject* entry = scene->findGameObjectByName("NameEntry"))
					if (NameEntryComponent* nameEntry = entry->getComponent<NameEntryComponent>())
						nameEntry->changeLetter(m_Delta);
		}

	private:
		int m_Delta;
	};

	class HighScoreCursorCommand final : public UndyneEngine::Command
	{
	public:
		explicit HighScoreCursorCommand(int delta) noexcept
			: m_Delta{ delta }
		{
		}

		void execute() override
		{
			if (GameState::phase() != GameState::Phase::EnterHighScore)
				return;
			if (UndyneEngine::Scene* scene = UndyneEngine::SceneManager::getActiveScene())
				if (UndyneEngine::GameObject* entry = scene->findGameObjectByName("NameEntry"))
					if (NameEntryComponent* nameEntry = entry->getComponent<NameEntryComponent>())
						nameEntry->moveCursor(m_Delta);
		}

	private:
		int m_Delta;
	};

	class HighScoreConfirmCommand final : public UndyneEngine::Command
	{
	public:
		void execute() override
		{
			switch (GameState::phase())
			{
			case GameState::Phase::EnterHighScore:
			{
				if (UndyneEngine::Scene* scene = UndyneEngine::SceneManager::getActiveScene())
					if (UndyneEngine::GameObject* entry = scene->findGameObjectByName("NameEntry"))
						if (NameEntryComponent* nameEntry = entry->getComponent<NameEntryComponent>())
							nameEntry->confirm();
				break;
			}
			case GameState::Phase::GameOver:
				GameState::returnToMenu();
				break;
			default:
				break;
			}
		}
	};
}
#endif
