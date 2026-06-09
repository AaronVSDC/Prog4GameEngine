#include "ScoreComponent.h"

namespace Digger
{
	void ScoreComponent::addPoints(int amount)
	{
		m_Score += amount;
		notify(*getOwner(), UndyneEngine::Event::ScoreChanged);
	}
}
