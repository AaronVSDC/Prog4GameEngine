#ifndef ENEMY_FACTORY_H
#define ENEMY_FACTORY_H
#include <glm/vec2.hpp>

//std
#include <memory>
#include <string>

namespace UndyneEngine { class GameObject; }

namespace Digger
{
	class EnemyFactory
	{
	public:
		static std::unique_ptr<UndyneEngine::GameObject> createMonster(
			const std::string& name, glm::vec2 worldPosition, float cellSize, bool manual = false);
	};
}
#endif
