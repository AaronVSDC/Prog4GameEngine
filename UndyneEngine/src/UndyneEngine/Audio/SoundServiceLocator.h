#ifndef SOUND_SERVICE_LOCATOR_H 
#define SOUND_SERVICE_LOCATOR_H
#include "SoundSystem.h"

//std
#include <memory>

namespace UndyneEngine::SoundServiceLocator
{
	SoundSystem& getSoundSystem();
	void registerSoundSystem(std::unique_ptr<SoundSystem> soundSystem);
}

#endif