#ifndef SCORE_DISPLAY_COMPONENT_H
#define SCORE_DISPLAY_COMPONENT_H
#include <UndyneEngine.h>

namespace Digger
{
	class ScoreDisplayComponent final : public UndyneEngine::BaseComponent, public UndyneEngine::Observer
	{
	public:
		~ScoreDisplayComponent() override;

		void start() override;
		void onNotify(UndyneEngine::GameObject& subject, UndyneEngine::Event event) override;
		void onSubjectDestroyed(UndyneEngine::Subject& subject) override;

	private:
		UndyneEngine::TextComponent* m_Text{ nullptr };
		UndyneEngine::Subject* m_Subject{ nullptr };
	};
}
#endif
