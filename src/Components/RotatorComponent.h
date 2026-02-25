#ifndef ROTATOR_COMPONENT_H
#define ROTATOR_COMPONENT_H
#include <glm/vec2.hpp>

#include "BaseComponent.h"

namespace dae
{
	class RotatorComponent final : public BaseComponent
	{
	public:
		explicit RotatorComponent(float radius, float radiansPerSecond = 1.f)
			: m_Speed(radiansPerSecond), m_Radius(radius) {
		} 
		 
		void Start(); 
		void Update(float) override;
		

	private:
		float m_Radius{ 0.f };       
		float m_Speed{ 0.f };
		float m_Angle{ 0.f };         

		glm::vec2 m_Center{};        
	};
}
#endif