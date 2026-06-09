#ifndef DIG_COMPONENT_H
#define DIG_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>

namespace Digger
{
	class LevelGridComponent;

	class DigComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float elapsedSec) override;

		bool dig(glm::ivec2 cell);
		bool digToward(glm::ivec2 direction);

		void setAutoDig(bool enabled) noexcept { m_AutoDig = enabled; }
		void setClearsObstacles(bool enabled) noexcept { m_ClearsObstacles = enabled; }

	private:
		float m_VisitMargin = 0.2f;
		bool m_AutoDig{ true };
		bool m_ClearsObstacles{ false };

		LevelGridComponent* m_Grid{ nullptr };
	};
}

#endif
