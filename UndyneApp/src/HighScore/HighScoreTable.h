#ifndef HIGH_SCORE_TABLE_H
#define HIGH_SCORE_TABLE_H

//std
#include <string>
#include <vector>

namespace Digger::HighScores
{
	struct Entry
	{
		std::string initials;
		int score{ 0 };
	};

	void load();
	void save();

	const std::vector<Entry>& entries();

	bool qualifies(int score);
	void insert(const std::string& initials, int score);
}
#endif
