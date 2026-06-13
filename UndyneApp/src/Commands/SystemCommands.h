#ifndef SYSTEM_COMMANDS_H
#define SYSTEM_COMMANDS_H
#include <UndyneEngine.h>
#include "../GameState/GameState.h"

namespace Digger
{
	class SkipLevelCommand final : public UndyneEngine::Command
	{
	public:
		void execute() override
		{
			GameState::skipLevel();
		}
	};

	class MuteToggleCommand final : public UndyneEngine::Command
	{
	public:
		void execute() override
		{
			UndyneEngine::SoundSystem& soundSystem = UndyneEngine::SoundServiceLocator::getSoundSystem();
			soundSystem.setMuted(!soundSystem.isMuted());
		}
	};
}
#endif
