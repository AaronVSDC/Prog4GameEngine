#include "Scene.h"

namespace UndyneEngine
{
	unsigned int Scene::m_ID = 0;

	Scene::Scene(const std::string& name): m_Name{name} {}

	void Scene::add(std::unique_ptr<GameObject> gameObject)
	{
		m_GameObjects.emplace_back(std::move(gameObject)); 
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