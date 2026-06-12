#ifndef PLAYER_FACTORY_H
#define PLAYER_FACTORY_H
#include <glm/vec2.hpp>

//std
#include <memory>
#include <string>

namespace UndyneEngine { class GameObject; }

namespace Digger
{
	class PlayerFactory
	{
	public:
		static std::unique_ptr<UndyneEngine::GameObject> createPlayer(
			const std::string& name, glm::vec2 worldPosition, float cellSize, bool primary);
	};
}
#endif
