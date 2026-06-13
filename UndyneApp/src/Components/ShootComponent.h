#ifndef SHOOT_COMPONENT_H
#define SHOOT_COMPONENT_H
#include <UndyneEngine.h>

namespace Digger
{
	class MoveComponent;
	class LevelGridComponent;

	class ShootComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float deltaTime) override;

		void requestFire() noexcept { m_WantsToFire = true; }

	private:
		void fire();

		const float FIRE_COOLDOWN{ 7.0f };

		bool m_WantsToFire{ false };
		float m_Cooldown{ 0.0f };
		MoveComponent* m_Move{ nullptr };
		const LevelGridComponent* m_Grid{ nullptr };
	};
}
#endif
