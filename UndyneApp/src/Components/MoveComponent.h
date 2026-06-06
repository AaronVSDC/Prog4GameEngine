#ifndef MOVE_COMPONENT_H
#define MOVE_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/glm.hpp>

namespace Digger
{
	class MoveComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float elapsedSec) override; 
		void setDirection(const glm::vec2& dir) { m_Direction = glm::normalize(dir); }

	private: 
		float m_Speed = 200.f; 
		glm::vec2 m_Direction = {};
		UndyneEngine::TextureComponent* m_TextureComponent{ nullptr }; 
	};
}


#endif