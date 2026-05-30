#include "SoundServiceLocator.h"

namespace UndyneEngine::SoundServiceLocator
{
    namespace
    {
        std::unique_ptr<SoundSystem>& storage()
        {
            static std::unique_ptr<SoundSystem> instance = std::make_unique<NullSoundSystem>();
            return instance;
        }
    }

    SoundSystem& getSoundSystem()
    {
        return *storage();
    }

    void registerSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
    {
        storage() = soundSystem
            ? std::move(soundSystem)
            : std::make_unique<NullSoundSystem>();
    }
}