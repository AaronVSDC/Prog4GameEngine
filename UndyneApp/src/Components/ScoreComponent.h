#ifndef SCORE_COMPONENT_H
#define SCORE_COMPONENT_H
#include <UndyneEngine.h>

namespace Digger
{
	class ScoreComponent final : public UndyneEngine::BaseComponent, public UndyneEngine::Subject
	{
	public:
		void update(float deltaTime) override;
		void addPoints(int amount);
		void collectEmerald();
		void collectGold();
		int getScore() const noexcept { return m_Score; }

	private:
		int m_Score{ 0 };
		int m_EmeraldStreak{ 0 };
		float m_StreakTimer{ 0.0f };
	};
}
#endif
