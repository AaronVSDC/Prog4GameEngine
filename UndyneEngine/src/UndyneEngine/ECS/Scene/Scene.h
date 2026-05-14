#ifndef SCENE_H
#define SCENE_H
#include "..\GameObject.h"

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

		void add(std::unique_ptr<GameObject> gameObject); 
		void update(float deltaTime);
		void fixedUpdate(float fixedTimeStep);
		void render() const;
		void start();


		//getters
		const std::string& getName() { return m_Name;  }
	private: 

		std::string m_Name; 
		std::vector<std::unique_ptr<GameObject>> m_GameObjects{}; 

		static unsigned int m_ID; 

	};
}

#endif