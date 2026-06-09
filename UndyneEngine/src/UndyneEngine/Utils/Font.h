#ifndef FONT_H
#define FONT_H

//std
#include <string>

struct TTF_Font;
namespace UndyneEngine
{
	class Font final
	{
	public:
		[[nodiscard]] TTF_Font* getFont() const;
		explicit Font(const std::string& fullPath, float size);
		~Font();

		Font(const Font&) = delete;
		Font(Font&&) = delete;
		Font& operator= (const Font&) = delete;
		Font& operator= (const Font&&) = delete;
	private:
		TTF_Font* m_Font;
	};
}



#endif