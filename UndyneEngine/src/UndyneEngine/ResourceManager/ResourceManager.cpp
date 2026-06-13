#include "ResourceManager.h"
#include "../Utils/Texture2D.h"
#include "../Utils/Font.h"
#include "../Renderer/Renderer.h"
#include "../Log/Log.h"

#include <SDL3/SDL.h>

//std
#include <map>
#include <SDL3_ttf/SDL_ttf.h>

namespace fs = std::filesystem; 

namespace UndyneEngine::ResourceManager
{
	namespace
	{
		std::filesystem::path m_DataPath;
		std::map<std::string, std::unique_ptr<Texture2D>> m_LoadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::unique_ptr<Font>> m_LoadedFonts;
	}
	void init()
	{
		//todo: if emscriptem we need another data path

		m_DataPath = "./Resources/"; 
		if (!fs::exists(m_DataPath))
			m_DataPath = "../Resources/"; 


		if (!TTF_Init())
			UDE_CORE_ERROR("Failed to load support for fonts: {}", SDL_GetError());
	}
	void destroy()
	{
		m_LoadedFonts.clear();
		m_LoadedTextures.clear();
		TTF_Quit();
	}
	Texture2D* loadTexture(const std::string& file)
	{
		const auto fullPath = m_DataPath / file;
		const auto filename = fs::path(fullPath).filename().string();
		if (m_LoadedTextures.find(filename) == m_LoadedTextures.end())
		{
			auto texture = std::make_unique<Texture2D>(fullPath.string());
			SDL_SetTextureScaleMode(texture->getSDLTexture(), SDL_SCALEMODE_NEAREST);
			m_LoadedTextures.insert(std::pair(filename, std::move(texture)));
		}
		return m_LoadedTextures.at(filename).get();

	}
	Font* loadFont(const std::string& file, uint8_t size)
	{
		const auto fullPath = m_DataPath / file;
		const auto filename = fs::path(fullPath).filename().string();
		const auto key = std::pair<std::string, uint8_t>(filename, size);
		if (m_LoadedFonts.find(key) == m_LoadedFonts.end())
			m_LoadedFonts.insert(std::pair(key, std::make_unique<Font>(fullPath.string(), size)));
		return m_LoadedFonts.at(key).get();
	}
	const std::filesystem::path& getDataPath() { return m_DataPath; }

}