#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace Digger::GameState
{
	enum class Phase { Menu, Playing, GameOver };
	enum class Mode { Normal, Coop, Versus };

	void init();
	void startGame(Mode mode);
	void winCurrentLevel();
	void enterGameOver();
	void returnToMenu();

	Phase phase() noexcept;
	Mode mode() noexcept;

	void pauseAction() noexcept;
	void resumeAction() noexcept;
	bool isActionPaused() noexcept;
}
#endif
