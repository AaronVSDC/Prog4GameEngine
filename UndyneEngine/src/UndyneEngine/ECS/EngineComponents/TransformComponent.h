#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include "..\BaseComponent.h"
#include "..\..\Core\Core.h"

#include <glm\vec3.hpp>

namespace UndyneEngine
{
	class UNDYNE_API TransformComponent final : public BaseComponent
	{
	public: 
		TransformComponent(glm::vec3 position = glm::vec3(0.0f)); 
		TransformComponent(float x, float y, float z); 
		[[nodiscard]] const glm::vec3& getPosition() const { return m_Position;  }
		void setPosition(float x, float y, float z = 0); 
		void setPosition(const glm::vec3& position); 
	private: 
		glm::vec3 m_Position; 
	};
}


#endif