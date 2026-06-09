#ifndef SCENE_H
#define SCENE_H
#include "../GameObject.h"

namespace UndyneEngine
{
	class Scene final
	{
	public:
		Scene(const std::string& name);
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

		void init();
		void add(std::unique_ptr<GameObject> gameObject);
		void remove(GameObject* gameObject);
		void removeAll();
		void cleanupMarked();
		void update(float deltaTime);
		void fixedUpdate(float fixedTimeStep);
		void render() const;
		void start();


		//getters
		const std::string& getName() { return m_Name;  }

		// Finds the first GameObject in the scene with this name, or nullptr. Components use
		// this to discover collaborators at runtime — then call getComponent<T>() on the
		// result — instead of having pointers injected into them.
		GameObject* findGameObjectByName(const std::string& name) const;

	private:

		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_GameObjects{};

	};
}

#endif
