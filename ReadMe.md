https://github.com/AaronVSDC/Prog4GameEngine.git
# UndyneEngine & Digger

This repo consists of 2 projects: **UndyneEngine** and **Digger**.
UndyneEngine is compiled as a static library and included in the Digger executable.

UndyneEngine uses the following programming patterns / implements the following:

- Logging
- Entity Component System
- Command
- Observer
- State
- ServiceLocator

## Entry Point

```cpp
using namespace UndyneEngine;
using namespace Digger;

UndyneEngine::Application* UndyneEngine::createApplication()
{
    return new UndyneApp();
}

void UndyneApp::load()
{
    GameState::init();
}
```

The EntryPoint of the application is defined on the engine side. When the client wants to use the engine, he makes an app, inherits from the `Application` class and calls `createApplication()`, and includes `<UndyneEngine.h>`.

## Logging

```cpp
namespace UndyneEngine
{
    namespace Log
    {
        void init();

        inline std::shared_ptr<spdlog::logger> s_CoreLogger;
        inline std::shared_ptr<spdlog::logger> s_ClientLogger;
    }
}

#ifdef UDE_DEBUG

#define UDE_CORE_CRITICAL(...) ::UndyneEngine::Log::s_CoreLogger->critical(__VA_ARGS__)
#define UDE_CORE_ERROR(...)    ::UndyneEngine::Log::s_CoreLogger->error(__VA_ARGS__)
#define UDE_CORE_WARN(...)     ::UndyneEngine::Log::s_CoreLogger->warn(__VA_ARGS__)
#define UDE_CORE_TRACE(...)    ::UndyneEngine::Log::s_CoreLogger->trace(__VA_ARGS__)
#define UDE_CORE_INFO(...)     ::UndyneEngine::Log::s_CoreLogger->info(__VA_ARGS__)

#define UDE_CRITICAL(...)      ::UndyneEngine::Log::s_ClientLogger->critical(__VA_ARGS__)
#define UDE_ERROR(...)         ::UndyneEngine::Log::s_ClientLogger->error(__VA_ARGS__)
#define UDE_WARN(...)          ::UndyneEngine::Log::s_ClientLogger->warn(__VA_ARGS__)
#define UDE_TRACE(...)         ::UndyneEngine::Log::s_ClientLogger->trace(__VA_ARGS__)
#define UDE_INFO(...)          ::UndyneEngine::Log::s_ClientLogger->info(__VA_ARGS__)

#define UDE_INITIALIZE_LOGGER  ::UndyneEngine::Log::init();

#endif

#ifdef UDE_RELEASE

#define UDE_CORE_CRITICAL
#define UDE_CORE_ERROR
#define UDE_CORE_WARN
#define UDE_CORE_TRACE
#define UDE_CORE_INFO

#define UDE_CRITICAL
#define UDE_ERROR
#define UDE_WARN
#define UDE_TRACE
#define UDE_INFO

#define UDE_INITIALIZE_LOGGER

#endif
```

One of the very first things I implemented was logging. I used the spdlog library because it was simple, lightweight and easy to use. Logging only happens via the macros so they get removed in release mode.

What you'll also see throughout the engine is the singleton pattern (small s) where I wanted single-instance abstraction. I preferred this over a class-based Singleton.

## Entity Component System

For the ECS, every Scene iterates over all GameObjects and all GameObjects iterate over all Components. The GameObject has methods like `addComponent()`, `removeComponent()`, etc. for the client to use.

## Renderer

```cpp
std::unique_ptr<Texture2D> createRenderTarget(int width, int height)
{
    SDL_Texture* target = SDL_CreateTexture(s_Renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!target)
    {
        UDE_CORE_ERROR("Failed to create render target: {}", SDL_GetError());
        return nullptr;
    }
    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(target, SDL_SCALEMODE_NEAREST);

    SDL_Texture* previousTarget = SDL_GetRenderTarget(s_Renderer);
    SDL_SetRenderTarget(s_Renderer, target);
    SDL_SetRenderDrawColor(s_Renderer, 0, 0, 0, 0);
    SDL_RenderClear(s_Renderer);
    SDL_SetRenderTarget(s_Renderer, previousTarget);

    return std::make_unique<Texture2D>(target);
}
```

Something specific about my renderer is that I needed a "brush effect" in my game, so the renderer has the ability to create a render target.

## Command

```cpp
InputManager::bindButtonCommand(KeyboardKey::Up, InputState::Pressed, std::make_unique<HighScoreLetterCommand>(1));
```

To create a command, you inherit from `Command`, `StickCommand`, or `TriggerCommand`, depending on whether you need the `vec2` value of the stick or the value from the trigger. Then you bind by calling `bindCommand`, `bindButtonCommand`, or `bindStickCommand`.

If you need to bind a controller, you first create a controller by calling `InputManager::addController()`.

## Observer

For the observer, you just inherit from either `Subject` or `Observer`, depending on whether you want to notify or be notified. The `Subject` can add or remove observers and notifies them all, and `Observer` just has `onNotify()`.

## ServiceLocator

For the sound system we have a ServiceLocator, which gets registered in the constructor of the application class. So anywhere in the game code you can just call the `SoundServiceLocator` and play/pause/load/... sounds, which happens concurrently.

## State

The state machine is a component. So when the client wants to add a state machine to their game object, they add the `StateMachineComponent` to it, and make individual states by inheriting from the `State` base class.
