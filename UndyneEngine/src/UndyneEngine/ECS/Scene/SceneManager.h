#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H  
#include "Scene.h"

//std
#include <string>

namespace UndyneEngine
{
	namespace SceneManager
	{
		void init();
		void destroy();
		Scene* createScene(const std::string& name);
		Scene* getActiveScene();

		void setActiveScene(Scene* scene); 
		void setActiveScene(const std::string& name); 

		void update(float deltaTime); 
		void fixedUpdate(float fixedDeltaTime); 
		void render(); 
		void start(); 

		void cleanupMarked();



	}
}

#endif