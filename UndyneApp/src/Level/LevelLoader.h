#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <glm/vec2.hpp>

//std
#include <string>
#include <vector>

namespace UndyneEngine { class Scene; }

namespace Digger
{

	struct LevelData
	{
		struct Placement
		{
			int column = 0;
			int row = 0;
			std::string texture;
		};

		int columns = 0;
		int rows = 0;
		float nativeCellSize = 0.0f;
		glm::ivec2 startCell{ 0, 0 };
		std::vector<glm::ivec2> dugCells;
		std::vector<Placement> entities;
	};

	class LevelLoader final
	{
	public:
		void load(int levelIndex, UndyneEngine::Scene& scene) const;

	private:
		int m_GridColumns = 15;
		int m_GridRows = 10;
		float m_NativeCellSize = 20.0f;


		bool parse(int levelIndex, LevelData& outData) const;
		bool readLevelGrid(int levelIndex, std::vector<std::string>& outRows) const;
	};
}

#endif
