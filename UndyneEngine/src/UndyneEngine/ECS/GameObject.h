#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "BaseComponent.h"

//std
#include <memory>
#include <string>
#include <vector>

namespace UndyneEngine
{

	class GameObject final
	{
	public: 
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void start();
		void update(float deltaTime);
		void fixedUpdate(float fixedTimeStep);
		void render() const; 

	private: 

		std::vector<std::unique_ptr<BaseComponent>> m_Components; 

	};


}



#endif