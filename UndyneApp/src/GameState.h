#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace Digger::GameState
{
	enum class Phase { Menu, Playing, GameOver };

	void init();
	void startGame();
	void winCurrentLevel();
	void enterGameOver();
	void returnToMenu();

	Phase phase() noexcept;
}
#endif
