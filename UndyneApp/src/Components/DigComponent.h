#ifndef DIG_COMPONENT_H
#define DIG_COMPONENT_H
#include <UndyneEngine.h>

namespace Digger
{
	class LevelGridComponent;

	class DigComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float elapsedSec) override;

	private:
		float m_VisitMargin = 0.2f;

		LevelGridComponent* m_Grid{ nullptr };
	};
}

#endif
