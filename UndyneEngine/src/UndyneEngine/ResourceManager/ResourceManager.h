#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

//std
#include <filesystem>
#include <string>
#include <memory>

namespace UndyneEngine
{
	class Texture2D; 
	class Font; 
	namespace ResourceManager
	{
		void init();
		void destroy();
		[[nodiscard]] Texture2D* loadTexture(const std::string& file);
		[[nodiscard]] Font* loadFont(const std::string& file, uint8_t size);
		[[nodiscard]] const std::filesystem::path& getDataPath();

	}
}




#endif

