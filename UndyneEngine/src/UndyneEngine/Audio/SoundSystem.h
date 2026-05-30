#ifndef I_SOUND_SYSTEM_H
#define I_SOUND_SYSTEM_H

#include "../Core/Core.h"

//std
#include <string>

namespace UndyneEngine
{
    class UNDYNE_API SoundSystem
    {
    public:
        virtual ~SoundSystem() = default;

        virtual void loadSound(const std::string& soundFile, const std::string& name) = 0;

        virtual void playSound(const std::string& name, bool loop = false) = 0;
        virtual void pauseSound(const std::string& name) = 0;
        virtual void resumeSound(const std::string& name) = 0;
        virtual void stopSound(const std::string& name) = 0;

        virtual void pauseAllSounds() = 0;
        virtual void resumeAllSounds() = 0;
        virtual void stopAllSounds() = 0;

        SoundSystem(const SoundSystem&) = delete;
        SoundSystem(SoundSystem&&) = delete;
        SoundSystem& operator=(const SoundSystem&) = delete;
        SoundSystem& operator=(SoundSystem&&) = delete;

    protected:
        SoundSystem() = default;
    };

    class UNDYNE_API NullSoundSystem final : public SoundSystem
    {
    public:
        NullSoundSystem() = default;
        ~NullSoundSystem() override = default;

        void loadSound(const std::string&, const std::string&) override {};

        void playSound(const std::string&, bool) override {};
        void pauseSound(const std::string&)      override {};
        void resumeSound(const std::string&)     override {};
        void stopSound(const std::string&)       override {};

        void pauseAllSounds()  override {};
        void resumeAllSounds() override {};
        void stopAllSounds()   override {};
    };
}

#endif