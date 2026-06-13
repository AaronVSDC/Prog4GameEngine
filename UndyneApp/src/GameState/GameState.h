#ifndef GAME_STATE_H
#define GAME_STATE_H

//std
#include <string>

namespace Digger::GameState
{
	enum class Phase { Menu, Playing, GameOver, EnterHighScore };
	enum class Mode { Normal, Coop, Versus };

	void init();
	void startGame(Mode mode);
	void winCurrentLevel();
	void skipLevel();
	void enterGameOver();
	void returnToMenu();
	void submitHighScore(const std::string& initials);

	Phase phase() noexcept;
	Mode mode() noexcept;

	void pauseAction() noexcept;
	void resumeAction() noexcept;
	bool isActionPaused() noexcept;
}
#endif
