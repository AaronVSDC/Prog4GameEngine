#ifndef ENEMY_COMPONENT_H
#define ENEMY_COMPONENT_H
#include <UndyneEngine.h>

//std
#include <string>

namespace Digger
{
	class LevelGridComponent;

	class EnemyComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;

		void setDeathTexture(const std::string& texturePath) { m_DeathTexture = texturePath; }

		bool isAlive() const noexcept { return m_Alive; }
		void markDead() noexcept { m_Alive = false; }

		void showDeathSprite();

	private:
		std::string m_DeathTexture{ "Sprites/NobbinDeath.png" };
		bool m_Alive{ true };
		LevelGridComponent* m_Grid{ nullptr };
	};
}
#endif
