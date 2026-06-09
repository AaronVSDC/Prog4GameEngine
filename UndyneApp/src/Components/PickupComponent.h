#ifndef PICKUP_COMPONENT_H
#define PICKUP_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>

namespace Digger
{
	class LevelGridComponent;

	class PickupComponent final : public UndyneEngine::BaseComponent
	{
	public:
		explicit PickupComponent(glm::ivec2 cell) noexcept;

		void start() override;
		void update(float deltaTime) override;

	private:
		glm::ivec2 m_Cell;
		UndyneEngine::GameObject* m_Player{ nullptr };
		const LevelGridComponent* m_Grid{ nullptr };
	};
}
#endif
