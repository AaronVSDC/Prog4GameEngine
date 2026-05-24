#include "Scene.h"
#include <cassert>
#include "..\..\Log\Log.h"

namespace UndyneEngine
{
	Scene::Scene(const std::string& name): m_Name{name} {}

	void Scene::init() {}

	void Scene::add(std::unique_ptr<GameObject> gameObject)
	{
		assert(gameObject && "Cannot add a null GameObject to the scene."); 
		m_GameObjects.emplace_back(std::move(gameObject));
	}

	GameObject* Scene::createGameObject(std::optional<std::string> name) //todo: check if this is a safe way of doing things. my reasoning right now is that gameobjects only get created in load() so adding them to the scene already and still returning a pointer is no issue. 
	{
		//todo: check that the client cannot assign the same name to a game object. 
		return m_GameObjects.emplace_back(std::make_unique<GameObject>(std::move(name))).get(); 
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
		std::erase_if(m_GameObjects, [](const std::unique_ptr<GameObject>& obj)
			{
				return obj->getMarkedForRemoval();
			});
	}

	void Scene::start()
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->start(); 
		}
	}
	void Scene::update(float deltaTime)
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->update(deltaTime);
		}
	}
	void Scene::fixedUpdate(float fixedTimeStep)
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->fixedUpdate(fixedTimeStep);
		}
	}
	void Scene::render() const
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->render();
		}
	}
}