#include "Scene.h"
#include <cassert>
#include "../../Log/Log.h"

namespace UndyneEngine
{
	Scene::Scene(const std::string& name): m_Name{name} {}

	void Scene::init() {}

	void Scene::add(std::unique_ptr<GameObject> gameObject)
	{
		assert(gameObject && "Cannot add a null GameObject to the scene.");
		gameObject->m_Scene = this;
		m_GameObjects.emplace_back(std::move(gameObject));
	}

	void Scene::remove(GameObject* gameObject)
	{
		std::erase_if(m_GameObjects,
			[gameObject](const auto& ptr) { return ptr.get() == gameObject; });
	}

	void Scene::removeAll()
	{
		m_GameObjects.clear(); 
	}

	void Scene::cleanupMarked()
	{
		std::erase_if(m_GameObjects,
			[](const std::unique_ptr<GameObject>& obj){return obj->getMarkedForRemoval();});
	}

	void Scene::start()
	{
		// Index-based: a component's start() may add new GameObjects (e.g. a level
		// builder), and those must be started too without invalidating iteration.
		for (std::size_t i = 0; i < m_GameObjects.size(); ++i)
			m_GameObjects[i]->start();
	}

	GameObject* Scene::findGameObjectByName(const std::string& name) const
	{
		for (const auto& gameObject : m_GameObjects)
		{
			if (gameObject->getName() == name)
				return gameObject.get();
		}
		return nullptr;
	}
	void Scene::update(float deltaTime)
	{
		for (std::size_t i = 0; i < m_GameObjects.size(); ++i)
			m_GameObjects[i]->update(deltaTime);
	}
	void Scene::fixedUpdate(float fixedTimeStep)
	{
		for (std::size_t i = 0; i < m_GameObjects.size(); ++i)
			m_GameObjects[i]->fixedUpdate(fixedTimeStep);
	}
	void Scene::render() const
	{
		for (std::size_t i = 0; i < m_GameObjects.size(); ++i)
			m_GameObjects[i]->render();
	}
}