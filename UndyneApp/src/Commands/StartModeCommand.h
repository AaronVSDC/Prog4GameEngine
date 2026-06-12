#ifndef START_MODE_COMMAND_H
#define START_MODE_COMMAND_H
#include <UndyneEngine.h>
#include "../GameState.h"

namespace Digger
{
	class StartModeCommand final : public UndyneEngine::Command
	{
	public:
		explicit StartModeCommand(GameState::Mode mode) noexcept
			: m_Mode{ mode }
		{
		}

		void execute() override
		{
			if (GameState::phase() == GameState::Phase::Menu)
				GameState::startGame(m_Mode);
		}

	private:
		GameState::Mode m_Mode;
	};
}
#endif
