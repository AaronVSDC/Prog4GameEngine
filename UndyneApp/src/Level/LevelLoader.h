#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <glm/vec4.hpp>
#include <UndyneEngine.h>

namespace Digger
{
	class LevelLoader final
	{
	public:
		LevelLoader() = default; 
		LevelLoader(const LevelLoader&) = delete; 
		LevelLoader(const LevelLoader&&) = delete; 
		LevelLoader& operator=(const LevelLoader&) = delete; 
		LevelLoader& operator=(const LevelLoader&&) = delete; 

		void loadLevel(int index, UndyneEngine::Scene& scene);
	private:
	    const int m_GridCollumns = 15; 
		const int m_GridRows = 10; 
		const float m_NativeCellSize = 20.f; 

		float m_PixelScale = 1.f; 
		float m_OriginX = 0.f; 
		float m_OriginY = 0.f; 

		bool readLevelGrid(int levelIndex, std::vector<std::string>& outRows) const; 
		void addBackgroundTile(int column, int row, UndyneEngine::Scene& scene) const;
		void addEntityTile(int column, int row, UndyneEngine::Scene& scene, const std::string& texturePath) const;

	};

}



#endif