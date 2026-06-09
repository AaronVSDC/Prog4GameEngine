#include "ScoreComponent.h"

namespace Digger
{
	void ScoreComponent::update(float deltaTime)
	{
		if (m_EmeraldStreak > 0)
		{
			m_StreakTimer -= deltaTime;
			if (m_StreakTimer <= 0.0f)
				m_EmeraldStreak = 0;
		}
	}

	void ScoreComponent::addPoints(int amount)
	{
		m_Score += amount;
		notify(*getOwner(), UndyneEngine::Event::ScoreChanged);
	}

	void ScoreComponent::collectEmerald()
	{
		m_Score += 25;
		m_StreakTimer = 0.4f;
		++m_EmeraldStreak;
		if (m_EmeraldStreak >= 8)
		{
			m_Score += 250;
			m_EmeraldStreak = 0;
		}
		notify(*getOwner(), UndyneEngine::Event::ScoreChanged);
	}

	void ScoreComponent::collectGold()
	{
		m_Score += 500;
		m_EmeraldStreak = 0;
		notify(*getOwner(), UndyneEngine::Event::ScoreChanged);
	}
}
