#ifndef MANUAL_CONTROL_COMPONENT_H
#define MANUAL_CONTROL_COMPONENT_H
#include <UndyneEngine.h>

namespace Digger
{
	class GridMovementComponent;
	class EnemyComponent;

	class ManualControlComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float deltaTime) override;

	private:
		GridMovementComponent* m_Mover{ nullptr };
		EnemyComponent* m_Enemy{ nullptr };
	};
}
#endif
