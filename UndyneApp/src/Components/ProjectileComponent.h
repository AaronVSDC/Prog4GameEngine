#ifndef PROJECTILE_COMPONENT_H
#define PROJECTILE_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>

namespace Digger
{
	class LevelGridComponent;

	class ProjectileComponent final : public UndyneEngine::BaseComponent
	{
	public:
		explicit ProjectileComponent(glm::ivec2 direction) noexcept;
		~ProjectileComponent() override;

		void start() override;
		void update(float deltaTime) override;

	private:
		bool hitEnemy(glm::ivec2 cell);
		void detonate();

		glm::ivec2 m_Direction;
		float m_Speed{ 0.0f };
		float m_MaxDistance{ 0.0f };
		float m_TravelledDistance{ 0.0f };
		const LevelGridComponent* m_Grid{ nullptr };
	};
}
#endif
