#include "NameEntryComponent.h"
#include "../GameState/GameState.h"

//std
#include <cstddef>

using namespace UndyneEngine;

namespace Digger
{
	void NameEntryComponent::start()
	{
		refreshDisplay();
	}

	void NameEntryComponent::changeLetter(int delta)
	{
		constexpr int letterCount = 26;
		char& letter = m_Initials[static_cast<std::size_t>(m_Cursor)];
		int value = (letter - 'A' + delta) % letterCount;
		if (value < 0)
			value += letterCount;
		letter = static_cast<char>('A' + value);
		refreshDisplay();
	}

	void NameEntryComponent::moveCursor(int delta)
	{
		m_Cursor += delta;
		if (m_Cursor < 0)
			m_Cursor = 0;
		if (m_Cursor > 2)
			m_Cursor = 2;
		refreshDisplay();
	}

	void NameEntryComponent::confirm()
	{
		GameState::submitHighScore(m_Initials);
	}

	void NameEntryComponent::refreshDisplay()
	{
		std::string display;
		for (int index = 0; index < 3; ++index)
		{
			const char letter = m_Initials[static_cast<std::size_t>(index)];
			if (index == m_Cursor)
			{
				display += '[';
				display += letter;
				display += ']';
			}
			else
			{
				display += ' ';
				display += letter;
				display += ' ';
			}
		}

		if (TextComponent* text = getOwner()->getComponent<TextComponent>())
			text->setText(display);
	}
}
