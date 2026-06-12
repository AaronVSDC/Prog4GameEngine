#ifndef MONSTER_SPAWNER_COMPONENT_H
#define MONSTER_SPAWNER_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>

//std
#include <vector>

namespace UndyneEngine { class Scene; }

namespace Digger
{
	class LevelGridComponent;

	class MonsterSpawnerComponent final
		: public UndyneEngine::BaseComponent
		, public UndyneEngine::Observer
	{
	public:
		~MonsterSpawnerComponent() override;

		void configure(glm::ivec2 spawnCell, int totalForLevel, int maxAlive, float interval, bool manual = false) noexcept;

		void start() override;
		void update(float deltaTime) override;

		void onNotify(UndyneEngine::GameObject& subject, UndyneEngine::Event event) override;
		void onSubjectDestroyed(UndyneEngine::Subject& subject) override;

		bool hasSpawnedAll() const noexcept { return m_SpawnedCount >= m_TotalForLevel; }

	private:
		void spawnMonster(UndyneEngine::Scene& scene);
		void resetWave();

		static constexpr float s_RespawnDelay{ 1.0f };

		glm::ivec2 m_SpawnCell{ 0, 0 };
		int m_TotalForLevel{ 0 };
		int m_MaxAlive{ 0 };
		int m_SpawnedCount{ 0 };
		float m_Interval{ 3.0f };
		float m_Timer{ 1.0f };
		bool m_Manual{ false };
		LevelGridComponent* m_Grid{ nullptr };
		std::vector<UndyneEngine::Subject*> m_LivesSubjects;
	};
}
#endif
