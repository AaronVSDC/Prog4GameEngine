#ifndef SCORE_COMPONENT_H
#define SCORE_COMPONENT_H
#include <UndyneEngine.h>

namespace Digger
{
	class ScoreComponent final : public UndyneEngine::BaseComponent, public UndyneEngine::Subject
	{
	public:
		void addPoints(int amount);
		int getScore() const noexcept { return m_Score; }

	private:
		int m_Score{ 0 };
	};
}
#endif
