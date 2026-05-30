#ifndef FONT_H
#define FONT_H
#include "../Core/Core.h"

//std
#include <string>

struct TTF_Font;
namespace UndyneEngine
{
	class UNDYNE_API Font final
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