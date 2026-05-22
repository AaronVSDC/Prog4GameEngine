#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H  
#include "Scene.h"
#include "..\..\Core\Core.h"

//std
#include <string>

namespace UndyneEngine
{
	namespace SceneManager
	{
		UNDYNE_API void init(); 
		UNDYNE_API Scene* createScene(const std::string& name); 
		UNDYNE_API Scene* getActiveScene(); 

		UNDYNE_API void setActiveScene(Scene* scene); 
		UNDYNE_API void setActiveScene(const std::string& name); 

		UNDYNE_API void update(float deltaTime); 
		UNDYNE_API void fixedUpdate(float fixedDeltaTime); 
		UNDYNE_API void render(); 
		UNDYNE_API void start(); 



	}
}

#endif