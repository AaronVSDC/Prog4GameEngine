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
		void die();

		int getLives() const noexcept { return m_Lives; }
		bool isGameOver() const noexcept { return m_GameOver; }

		void onNotify(UndyneEngine::GameObject& subject, UndyneEngine::Event event) override;
		void onSubjectDestroyed(UndyneEngine::Subject& subject) override;

	private:
		void showGravestone(bool visible);

		int m_Lives{ 3 };
		int m_NextExtraLife{ 20000 };
		bool m_GameOver{ false };
		bool m_Dying{ false };
		float m_DeathTimer{ 0.0f };
		float m_DeathDuration{ 1.5f };
		UndyneEngine::Subject* m_Score{ nullptr };
	};
}
#endif
