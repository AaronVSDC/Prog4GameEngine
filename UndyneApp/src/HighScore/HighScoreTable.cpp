#include "HighScoreTable.h"
#include <UndyneEngine.h>

//std
#include <algorithm>
#include <cstddef>
#include <fstream>

namespace Digger::HighScores
{
	namespace
	{
		constexpr std::size_t s_MaxEntries{ 10 };
		std::vector<Entry> s_Entries;

		void sortAndTrim()
		{
			std::stable_sort(s_Entries.begin(), s_Entries.end(),
				[](const Entry& left, const Entry& right) { return left.score > right.score; });
			if (s_Entries.size() > s_MaxEntries)
				s_Entries.resize(s_MaxEntries);
		}

		void seedDefaults()
		{
			s_Entries = {
				{ "DIG", 5000 }, { "GER", 4500 }, { "ACE", 4000 }, { "BOB", 3500 }, { "CAT", 3000 },
				{ "DOG", 2500 }, { "EVE", 2000 }, { "FOX", 1500 }, { "GUS", 1000 }, { "HAL", 500 }
			};
		}
	}

	void load()
	{
		s_Entries.clear();

		const auto path = UndyneEngine::ResourceManager::getDataPath() / "HighScores.txt";
		std::ifstream file{ path };
		if (file.is_open())
		{
			std::string initials;
			int score{ 0 };
			while (file >> initials >> score)
				s_Entries.push_back({ initials, score });
		}

		if (s_Entries.empty())
			seedDefaults();
		sortAndTrim();
	}

	void save()
	{
		const auto path = UndyneEngine::ResourceManager::getDataPath() / "HighScores.txt";
		std::ofstream file{ path, std::ios::trunc };
		if (!file.is_open())
		{
			UDE_ERROR("HighScores: cannot write {}", path.string());
			return;
		}

		for (const Entry& entry : s_Entries)
			file << entry.initials << ' ' << entry.score << '\n';
	}

	const std::vector<Entry>& entries()
	{
		return s_Entries;
	}

	bool qualifies(int score)
	{
		if (score <= 0)
			return false;
		if (s_Entries.size() < s_MaxEntries)
			return true;
		return score > s_Entries.back().score;
	}

	void insert(const std::string& initials, int score)
	{
		s_Entries.push_back({ initials, score });
		sortAndTrim();
	}
}
