#include "HighScoreTable.h"
#include <UndyneEngine.h>

//std
#include <algorithm>
#include <fstream>

namespace Digger
{
	void HighScoreTable::load()
	{
		m_Entries.clear();

		const auto path = UndyneEngine::ResourceManager::getDataPath() / "HighScores.txt";
		std::ifstream file{ path };
		if (file.is_open())
		{
			std::string initials;
			int score{ 0 };
			while (file >> initials >> score)
				m_Entries.push_back({ initials, score });
		}

		if (m_Entries.empty())
			seedDefaults();
		sortAndTrim();
	}

	void HighScoreTable::save() const
	{
		const auto path = UndyneEngine::ResourceManager::getDataPath() / "HighScores.txt";
		std::ofstream file{ path, std::ios::trunc };
		if (!file.is_open())
		{
			UDE_ERROR("HighScores: cannot write {}", path.string());
			return;
		}

		for (const Entry& entry : m_Entries)
			file << entry.initials << ' ' << entry.score << '\n';
	}

	bool HighScoreTable::qualifies(int score) const
	{
		if (score <= 0)
			return false;
		if (m_Entries.size() < MAX_ENTRIES)
			return true;
		return score > m_Entries.back().score;
	}

	void HighScoreTable::insert(const std::string& initials, int score)
	{
		m_Entries.push_back({ initials, score });
		sortAndTrim();
	}

	void HighScoreTable::sortAndTrim()
	{
		std::stable_sort(m_Entries.begin(), m_Entries.end(),
			[](const Entry& left, const Entry& right) { return left.score > right.score; });
		if (m_Entries.size() > MAX_ENTRIES)
			m_Entries.resize(MAX_ENTRIES);
	}

	void HighScoreTable::seedDefaults()
	{
		m_Entries = {
			{ "DIG", 5000 }, { "GER", 4500 }, { "ACE", 4000 }, { "BOB", 3500 }, { "CAT", 3000 },
			{ "DOG", 2500 }, { "EVE", 2000 }, { "FOX", 1500 }, { "GUS", 1000 }, { "HAL", 500 }
		};
	}
}
