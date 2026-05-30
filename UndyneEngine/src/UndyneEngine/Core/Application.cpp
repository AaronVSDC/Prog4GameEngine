#include "Application.h"
#include "Window.h"
#include "../Log/Log.h"
#include "../Input/InputManager.h"
#include "../ECS/Scene/SceneManager.h"
#include "../Renderer/Renderer.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Audio/SoundServiceLocator.h"
#include "../Audio/SDLSoundSystem.h"
//std
#include <chrono>

namespace UndyneEngine
{
	Application::Application()
	{
		UDE_INITIALIZE_LOGGER; 
		m_Window = std::make_unique<Window>();
		Renderer::init(m_Window->getHandle()); 
		ResourceManager::init(); 
		InputManager::init(); 
		SoundServiceLocator::registerSoundSystem(std::make_unique<SDLSoundSystem>()); 
	}

	Application::~Application()
	{
		InputManager::destroy(); 
		Renderer::destroy(); 
		ResourceManager::destroy();  
	}
	void Application::run()
	{
		load();
		bool quit = false;

		constexpr int desiredFPS{ 60 }; 
		constexpr int frameTimeMs{ 1000 / desiredFPS }; 

		const float fixedTimeStep{ 0.02f }; 

		auto lastTime = std::chrono::high_resolution_clock::now(); 
		float lag = 0.f; 

		SceneManager::init();
		SceneManager::start(); 
		while (!quit)
		{
			SceneManager::cleanupMarked();

			const auto currentTime = std::chrono::high_resolution_clock::now(); 
			const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count(); 
			lastTime = currentTime; 
			lag += deltaTime; 

			quit = !InputManager::processInput(); 

			while (lag >= fixedTimeStep)
			{
				SceneManager::fixedUpdate(fixedTimeStep); 
				lag -= fixedTimeStep; 
			}
			SceneManager::update(deltaTime); 
			Renderer::render();

			const auto sleepTime = currentTime + std::chrono::milliseconds(frameTimeMs) - std::chrono::high_resolution_clock::now(); 
			std::this_thread::sleep_for(sleepTime); 
		}

	}
}