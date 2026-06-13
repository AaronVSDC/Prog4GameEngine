#ifndef HIGH_SCORE_TABLE_H
#define HIGH_SCORE_TABLE_H

//std
#include <cstddef>
#include <string>
#include <vector>

namespace Digger
{
	class HighScoreTable final
	{
	public:
		struct Entry
		{
			std::string initials;
			int score{ 0 };
		};

		void load();
		void save() const;

		const std::vector<Entry>& entries() const noexcept { return m_Entries; }

		bool qualifies(int score) const;
		void insert(const std::string& initials, int score);

	private:
		void sortAndTrim();
		void seedDefaults();

		const std::size_t MAX_ENTRIES{ 10 };
		std::vector<Entry> m_Entries;
	};
}
#endif
