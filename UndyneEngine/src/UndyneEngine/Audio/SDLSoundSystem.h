#ifndef SDL_SOUND_SYSTEM_H
#define SDL_SOUND_SYSTEM_H

#include "SoundSystem.h"

//std
#include <memory>

namespace UndyneEngine
{
    class SDLSoundSystem final : public SoundSystem
    {
    public:
        SDLSoundSystem();
        ~SDLSoundSystem() override;

        void loadSound(const std::string& name, const std::string& soundFile) override;

        void playSound(const std::string& name, bool loop = false) override;
        void pauseSound(const std::string& name) override;
        void resumeSound(const std::string& name) override;
        void stopSound(const std::string& name) override;

        void pauseAllSounds()  override;
        void resumeAllSounds() override;
        void stopAllSounds()   override;
    private:
        class SDLSoundSystemImpl;
        std::unique_ptr<SDLSoundSystemImpl> m_pImpl;
    };
}

#endif