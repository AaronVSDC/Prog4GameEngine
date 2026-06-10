#ifndef LEVEL_CONTROLLER_H
#define LEVEL_CONTROLLER_H
#include <UndyneEngine.h>

namespace Digger
{
	class LevelController final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float deltaTime) override;

	private:
		bool m_HadGems{ false };
		bool m_Resolved{ false };
	};
}
#endif
