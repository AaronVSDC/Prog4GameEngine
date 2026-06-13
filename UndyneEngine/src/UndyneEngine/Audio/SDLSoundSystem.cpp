#include "SDLSoundSystem.h"
#include "../Log/Log.h"

#include <SDL3_mixer/SDL_mixer.h>

//std
#include <algorithm>
#include <condition_variable>
#include <filesystem>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

namespace UndyneEngine
{
    namespace
    {
        struct MixAudioDeleter
        {
            void operator()(MIX_Audio* audio) const noexcept
            {
                if (audio != nullptr) MIX_DestroyAudio(audio);
            }
        };
        using MixAudioPtr = std::unique_ptr<MIX_Audio, MixAudioDeleter>;
    }

    class SDLSoundSystem::SDLSoundSystemImpl final
    {
    public:
        SDLSoundSystemImpl()
            : m_WorkerThread{ &SDLSoundSystemImpl::workerLoop, this }
        {
        }

        ~SDLSoundSystemImpl()
        {
            {
                std::scoped_lock lock{ m_QueueMutex };
                m_Quitting = true;
                m_RequestQueue.push({ RequestType::Quit, {}, {}, nullptr, false });
            }
            m_QueueCv.notify_all();
            if (m_WorkerThread.joinable())
                m_WorkerThread.join();
        }

        void requestLoad(const std::string& soundFile, const std::string& name) { enqueue({ RequestType::Load, name, soundFile, nullptr, false }); }
        void requestPlay(const std::string& name, bool loop) { enqueue({ RequestType::Play, name, {}, nullptr, loop }); }
        void requestPause(const std::string& name) { enqueue({ RequestType::PauseName, name, {}, nullptr, false }); }
        void requestResume(const std::string& name) { enqueue({ RequestType::ResumeName,name, {},nullptr, false }); }
        void requestStop(const std::string& name) { enqueue({ RequestType::StopName,name, {}, nullptr, false }); }
        void requestPauseAll() { enqueue({ RequestType::PauseAll,  {},{}, nullptr, false }); }
        void requestResumeAll() { enqueue({ RequestType::ResumeAll, {},{},nullptr, false }); }
        void requestStopAll() { enqueue({ RequestType::StopAll,   {},{},nullptr, false }); }
        void requestSetMute(bool muted) { enqueue({ muted ? RequestType::Mute : RequestType::Unmute, {}, {}, nullptr, false }); }

    private:
        enum class RequestType
        {
            Load,
            Play,
            PauseName, ResumeName, StopName,
            PauseAll, ResumeAll, StopAll,
            Mute, Unmute,
            TrackFinished,
            Quit
        };

        struct SoundRequest
        {
            RequestType type;
            std::string name;
            std::string soundFile; 
            MIX_Track* track;
            bool        loop = false;
        };

        void enqueue(SoundRequest request)
        {
            {
                std::scoped_lock lock{ m_QueueMutex };
                if (m_Quitting) return;
                m_RequestQueue.push(std::move(request));
            }
            m_QueueCv.notify_one();
        }

        static void SDLCALL onTrackStopped(void* userdata, MIX_Track* track)
        {
            auto* self = static_cast<SDLSoundSystemImpl*>(userdata);
            self->enqueue({ RequestType::TrackFinished, {}, {}, track, false });
        }

        void workerLoop()
        {
            const bool audioReady = initAudio();

            while (true)
            {
                SoundRequest request;
                {
                    std::unique_lock lock{ m_QueueMutex };
                    m_QueueCv.wait(lock, [this] { return !m_RequestQueue.empty(); });
                    request = std::move(m_RequestQueue.front());
                    m_RequestQueue.pop();
                }

                if (request.type == RequestType::Quit) break;
                if (audioReady) dispatch(request);
            }

            if (audioReady) shutdownAudio();
        }

        void dispatch(const SoundRequest& request)
        {
            switch (request.type)
            {
            case RequestType::Load:          handleLoad(request.soundFile, request.name);      break;
            case RequestType::Play:          handlePlay(request.name, request.loop);           break;
            case RequestType::PauseName:     handlePause(request.name);                        break;
            case RequestType::ResumeName:    handleResume(request.name);                       break;
            case RequestType::StopName:      handleStop(request.name);                         break;
            case RequestType::PauseAll:      handlePauseAll();                                 break;
            case RequestType::ResumeAll:     handleResumeAll();                                break;
            case RequestType::StopAll:       handleStopAll();                                  break;
            case RequestType::Mute:          handleMute();                                     break;
            case RequestType::Unmute:        handleUnmute();                                   break;
            case RequestType::TrackFinished: handleTrackFinished(request.track);               break;
            case RequestType::Quit:                                                            break;
            }
        }

        bool initAudio()
        {
            if (!MIX_Init())
            {
                UDE_CORE_ERROR("Failed to init SDL_mixer: {}", SDL_GetError());
                return false;
            }
            m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
            if (m_Mixer == nullptr)
            {
                UDE_CORE_ERROR("Failed to create mixer device: {}", SDL_GetError());
                MIX_Quit();
                return false;
            }
            UDE_CORE_INFO("SDL_mixer initialized.");
            return true;
        }

        void shutdownAudio()
        {
            MIX_StopAllTracks(m_Mixer, 0);
            m_ActiveTracks.clear();
            m_TrackOwner.clear();
            m_Audios.clear();

            if (m_Mixer != nullptr)
            {
                MIX_DestroyMixer(m_Mixer);
                m_Mixer = nullptr;
            }
            MIX_Quit();
            UDE_CORE_INFO("SDL_mixer shut down.");
        }

        void handleLoad(const std::string& soundFile, const std::string& name)
        {
            if (m_Audios.find(name) != m_Audios.end())
            {
                UDE_CORE_WARN("loadSound: name '{}' is already in use; ignoring.", name);
                return;
            }

            const auto fullPath = resolveDataPath() / soundFile;
            MixAudioPtr audio{ MIX_LoadAudio(m_Mixer, fullPath.string().c_str(), true) };
            if (audio == nullptr)
            {
                UDE_CORE_ERROR("Failed to load '{}' as '{}': {}", soundFile, name, SDL_GetError());
                return;
            }

            UDE_CORE_INFO("Loaded sound '{}' as '{}'.", soundFile, name);
            m_Audios.insert_or_assign(name, std::move(audio));
        }

        void handlePlay(const std::string& name, bool loop)
        {
            const auto iter = m_Audios.find(name);
            if (iter == m_Audios.end())
            {
                UDE_CORE_ERROR("playSound: '{}' not loaded.", name);
                return;
            }

            MIX_Track* track = MIX_CreateTrack(m_Mixer);
            if (track == nullptr)
            {
                UDE_CORE_ERROR("MIX_CreateTrack failed for '{}': {}", name, SDL_GetError());
                return;
            }
            if (!MIX_SetTrackAudio(track, iter->second.get()))
            {
                UDE_CORE_ERROR("MIX_SetTrackAudio failed for '{}': {}", name, SDL_GetError());
                MIX_DestroyTrack(track);
                return;
            }
            MIX_SetTrackStoppedCallback(track, &SDLSoundSystemImpl::onTrackStopped, this);

            SDL_PropertiesID options = 0;
            if (loop)
            {
                options = SDL_CreateProperties();
                SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
            }
            const bool ok = MIX_PlayTrack(track, options);
            if (options != 0) SDL_DestroyProperties(options);

            if (!ok)
            {
                UDE_CORE_ERROR("MIX_PlayTrack failed for '{}': {}", name, SDL_GetError());
                MIX_DestroyTrack(track);
                return;
            }

            if (loop) UDE_CORE_INFO("Playing sound '{}' on loop.", name);
            else      UDE_CORE_INFO("Playing sound '{}'.", name);

            m_ActiveTracks[name].push_back(track);
            m_TrackOwner[track] = name;
        }

        void handlePause(const std::string& name)
        {
            const auto iter = m_ActiveTracks.find(name);
            if (iter == m_ActiveTracks.end())
            {
                UDE_CORE_WARN("pauseSound: '{}' has no active tracks.", name);
                return;
            }
            for (MIX_Track* track : iter->second) MIX_PauseTrack(track);
            UDE_CORE_INFO("Paused sound '{}' ({} track(s)).", name, iter->second.size());
        }

        void handleResume(const std::string& name)
        {
            const auto iter = m_ActiveTracks.find(name);
            if (iter == m_ActiveTracks.end())
            {
                UDE_CORE_WARN("resumeSound: '{}' has no active tracks.", name);
                return;
            }
            for (MIX_Track* track : iter->second) MIX_ResumeTrack(track);
            UDE_CORE_INFO("Resumed sound '{}' ({} track(s)).", name, iter->second.size());
        }

        void handleStop(const std::string& name)
        {
            const auto iter = m_ActiveTracks.find(name);
            if (iter == m_ActiveTracks.end())
            {
                UDE_CORE_WARN("stopSound: '{}' has no active tracks.", name);
                return;
            }
            for (MIX_Track* track : iter->second) MIX_StopTrack(track, 0);
            UDE_CORE_INFO("Stopped sound '{}' ({} track(s)).", name, iter->second.size());
        }

        void handlePauseAll()
        {
            MIX_PauseAllTracks(m_Mixer);
            UDE_CORE_INFO("Paused all sounds.");
        }

        void handleResumeAll()
        {
            MIX_ResumeAllTracks(m_Mixer);
            UDE_CORE_INFO("Resumed all sounds.");
        }

        void handleStopAll()
        {
            MIX_StopAllTracks(m_Mixer, 0);
            UDE_CORE_INFO("Stopped all sounds.");
        }

        void handleMute()
        {
            MIX_SetMixerGain(m_Mixer, 0.0f);
            UDE_CORE_INFO("Muted all sounds.");
        }

        void handleUnmute()
        {
            MIX_SetMixerGain(m_Mixer, 1.0f);
            UDE_CORE_INFO("Unmuted all sounds.");
        }

        void handleTrackFinished(MIX_Track* track)
        {
            const auto ownerIter = m_TrackOwner.find(track);
            if (ownerIter == m_TrackOwner.end()) return;

            const std::string name = std::move(ownerIter->second);
            m_TrackOwner.erase(ownerIter);

            auto& tracks = m_ActiveTracks[name];
            tracks.erase(std::remove(tracks.begin(), tracks.end(), track), tracks.end());
            if (tracks.empty()) m_ActiveTracks.erase(name);

            MIX_DestroyTrack(track);
            UDE_CORE_INFO("Finished sound '{}'.", name);
        }

        static std::filesystem::path resolveDataPath()
        {
            std::filesystem::path dataPath{ "./Resources/" };
            if (!std::filesystem::exists(dataPath))
                dataPath = "../Resources/";
            return dataPath;
        }

        std::queue<SoundRequest> m_RequestQueue;
        std::mutex               m_QueueMutex;
        std::condition_variable  m_QueueCv;
        bool                     m_Quitting{ false };

        MIX_Mixer* m_Mixer{ nullptr };
        std::unordered_map<std::string, MixAudioPtr>             m_Audios;        
        std::unordered_map<std::string, std::vector<MIX_Track*>> m_ActiveTracks;  
        std::unordered_map<MIX_Track*, std::string>              m_TrackOwner;   

        std::thread m_WorkerThread;
    };

    SDLSoundSystem::SDLSoundSystem()
        : m_pImpl{ std::make_unique<SDLSoundSystemImpl>() }
    {
    }

    SDLSoundSystem::~SDLSoundSystem() = default;

    void SDLSoundSystem::loadSound(const std::string& soundFile, const std::string& name) { m_pImpl->requestLoad(soundFile, name); }
    void SDLSoundSystem::playSound(const std::string& name, bool loop) { m_pImpl->requestPlay(name, loop); }
    void SDLSoundSystem::pauseSound(const std::string& name) { m_pImpl->requestPause(name); }
    void SDLSoundSystem::resumeSound(const std::string& name) { m_pImpl->requestResume(name); }
    void SDLSoundSystem::stopSound(const std::string& name) { m_pImpl->requestStop(name); }
    void SDLSoundSystem::pauseAllSounds() { m_pImpl->requestPauseAll(); }
    void SDLSoundSystem::resumeAllSounds() { m_pImpl->requestResumeAll(); }
    void SDLSoundSystem::stopAllSounds() { m_pImpl->requestStopAll(); }
    void SDLSoundSystem::setMuted(bool muted) { m_Muted = muted; m_pImpl->requestSetMute(muted); }
    bool SDLSoundSystem::isMuted() const { return m_Muted; }
}