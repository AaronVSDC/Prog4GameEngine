#ifndef LIVES_COMPONENT_H
#define LIVES_COMPONENT_H
#include <UndyneEngine.h>

namespace Digger
{
	class LivesComponent final
		: public UndyneEngine::BaseComponent
		, public UndyneEngine::Subject
		, public UndyneEngine::Observer
	{
	public:
		~LivesComponent() override;

		void start() override;
		void update(float deltaTime) override;
		void die(UndyneEngine::GameObject& dyingPlayer);

		int getLives() const noexcept { return m_Lives; }
		bool isGameOver() const noexcept { return m_GameOver; }

		void onNotify(UndyneEngine::GameObject& subject, UndyneEngine::Event event) override;
		void onSubjectDestroyed(UndyneEngine::Subject& subject) override;

	private:
		void beginDeath();
		void respawn();
		void showGravestone(bool visible);

		static constexpr float s_DeathDelay{ 1.0f };

		int m_Lives{ 3 };
		int m_NextExtraLife{ 20000 };
		bool m_GameOver{ false };
		bool m_DeathPending{ false };
		float m_DeathTimer{ 0.0f };
		bool m_Dying{ false };
		UndyneEngine::GameObject* m_DyingPlayer{ nullptr };
		UndyneEngine::AnimationComponent* m_DeathAnimation{ nullptr };
		UndyneEngine::Subject* m_Score{ nullptr };
	};
}
#endif
