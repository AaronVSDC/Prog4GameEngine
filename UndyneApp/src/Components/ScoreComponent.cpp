#include "ScoreComponent.h"

namespace Digger
{
	void ScoreComponent::start()
	{
		auto& soundSystem = UndyneEngine::SoundServiceLocator::getSoundSystem(); 
		for (int note = 0; note < 8; ++note)
			soundSystem.loadSound("Audio/emerald" + std::to_string(note) + ".wav", "emerald" + std::to_string(note));
	}
	void ScoreComponent::update(float deltaTime)
	{
		if (m_RingingNote >= 0)              
		{
			m_StreakTimer -= deltaTime;
			if (m_StreakTimer <= 0.0f)       
			{
				m_EmeraldStreak = 0;
				m_RingingNote = -1;        
			}
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

		constexpr float emeraldNoteDuration = 0.731f; 
		auto& sound = UndyneEngine::SoundServiceLocator::getSoundSystem();
		const int note = std::min(m_EmeraldStreak, 7);

		if (m_RingingNote >= 0) sound.stopSound("emerald" + std::to_string(m_RingingNote));
		sound.playSound("emerald" + std::to_string(note));
		m_RingingNote = note;
		m_StreakTimer = emeraldNoteDuration;                  

		++m_EmeraldStreak;
		if (m_EmeraldStreak >= 8) { m_Score += 250; m_EmeraldStreak = 0; }
		notify(*getOwner(), UndyneEngine::Event::ScoreChanged);
	}

	void ScoreComponent::collectGold()
	{
		m_Score += 500;
		m_EmeraldStreak = 0;
		notify(*getOwner(), UndyneEngine::Event::ScoreChanged);
	}
}
