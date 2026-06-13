#ifndef NAME_ENTRY_COMPONENT_H
#define NAME_ENTRY_COMPONENT_H
#include <UndyneEngine.h>

//std
#include <string>

namespace Digger
{
	class NameEntryComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;

		void changeLetter(int delta);
		void moveCursor(int delta);
		void confirm();

	private:
		void refreshDisplay();

		std::string m_Initials{ "AAA" };
		int m_Cursor{ 0 };
	};
}
#endif
