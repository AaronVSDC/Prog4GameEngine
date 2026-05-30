#ifndef SOUND_SERVICE_LOCATOR_H 
#define SOUND_SERVICE_LOCATOR_H
#include "../Core/Core.h"
#include "SoundSystem.h"

//std
#include <memory>

namespace UndyneEngine::SoundServiceLocator
{
	UNDYNE_API SoundSystem& getSoundSystem();
	UNDYNE_API void registerSoundSystem(std::unique_ptr<SoundSystem> soundSystem);
}

#endif