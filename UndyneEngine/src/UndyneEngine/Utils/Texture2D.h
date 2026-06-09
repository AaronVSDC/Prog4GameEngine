#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H
#include <glm/vec2.hpp>

//std
#include <string>

struct SDL_Texture; 
namespace UndyneEngine
{
	class Texture2D final
	{
	public:
		[[nodiscard]] SDL_Texture* getSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		explicit Texture2D(const std::string& fullPath);
		~Texture2D();

		[[nodiscard]] glm::vec2 getSize() const;

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;
	private:
		SDL_Texture* m_Texture;
	};
}


#endif