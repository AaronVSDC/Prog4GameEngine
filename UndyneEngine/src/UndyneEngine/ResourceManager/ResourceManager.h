#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "../Core/Core.h"

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
		UNDYNE_API void init();
		UNDYNE_API void destroy();
		UNDYNE_API [[nodiscard]] std::shared_ptr<Texture2D> loadTexture(const std::string& file);
		UNDYNE_API [[nodiscard]] std::shared_ptr<Font> loadFont(const std::string& file, uint8_t size);
		UNDYNE_API [[nodiscard]] const std::filesystem::path& getDataPath();

	}
}




#endif

