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
		static constexpr float s_WinDelay{ 1.5f };

		bool m_HadGems{ false };
		bool m_Resolved{ false };
		bool m_WinPending{ false };
		float m_WinTimer{ 0.0f };
	};
}
#endif
