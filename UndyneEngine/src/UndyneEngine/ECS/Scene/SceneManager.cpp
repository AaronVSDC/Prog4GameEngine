#include "SceneManager.h" 
#include "../../Log/Log.h"

//std
#include <vector>
#include <memory>

namespace UndyneEngine::SceneManager
{
	namespace
	{
		std::vector<std::unique_ptr<Scene>> s_Scenes; 
		Scene* s_ActiveScene = nullptr; 
	}

	void init()
	{
		if (!s_ActiveScene)
		{
			UDE_CORE_WARN("SceneManager has no active scene."); 
			return; 
		}
		s_ActiveScene->init(); 
	}
	void destroy()
	{
		s_Scenes.clear();
		s_ActiveScene = nullptr;
	}

	Scene* createScene(const std::string& name)
	{
		for (const auto& scene : s_Scenes)
		{
			if (scene->getName() == name)
			{
				UDE_CORE_ERROR("Scene with name \"{}\" already exists", name); 
				return nullptr; 
			}
		}

		Scene* scene = s_Scenes.emplace_back(std::make_unique<Scene>(name)).get();
		if (!s_ActiveScene) s_ActiveScene = scene; 
		return scene; 
	}

	Scene* getActiveScene()	{ return s_ActiveScene; }

	void setActiveScene(Scene* scene) { s_ActiveScene = scene;  }

	void setActiveScene(const std::string& name)
	{
		for (const auto& scene : s_Scenes)
			if (scene->getName() == name) { s_ActiveScene = scene.get(); return; }
		
	}

	void update(float deltaTime)
	{
		if (s_ActiveScene) s_ActiveScene->update(deltaTime);	//todo: maybe optimize this? am not the BIGGEST fan of this way of doing things although you basically get the pointercheck for free. 
	}

	void fixedUpdate(float fixedDeltaTime)
	{
		if (s_ActiveScene) s_ActiveScene->fixedUpdate(fixedDeltaTime);
	}

	void render()
	{
		if (s_ActiveScene) s_ActiveScene->render();
	}

	void start()
	{
		if (s_ActiveScene) s_ActiveScene->start();
	}
	void cleanupMarked()
	{
		if (s_ActiveScene) s_ActiveScene->cleanupMarked(); 
	}
}