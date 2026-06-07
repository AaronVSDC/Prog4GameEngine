#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <UndyneEngine.h>

namespace Digger
{
	class DigField;

	class LevelLoader final
	{
	public:
		LevelLoader() = default; 
		LevelLoader(const LevelLoader&) = delete; 
		LevelLoader(const LevelLoader&&) = delete; 
		LevelLoader& operator=(const LevelLoader&) = delete; 
		LevelLoader& operator=(const LevelLoader&&) = delete; 

		void loadLevel(int index, UndyneEngine::Scene& scene);

		DigField* getDigField() const noexcept { return m_DigField; }
		glm::ivec2 getStartCell() const noexcept { return m_StartCell; }
	private:
	    const int m_GridCollumns = 15; 
		const int m_GridRows = 10; 
		const float m_NativeCellSize = 20.f;

		DigField* m_DigField = nullptr;
		glm::ivec2 m_StartCell{ 0, 0 };

		bool readLevelGrid(int levelIndex, std::vector<std::string>& outRows) const;
		void addEntityTile(int column, int row, UndyneEngine::Scene& scene, const std::string& texturePath) const;

	};

}



#endif