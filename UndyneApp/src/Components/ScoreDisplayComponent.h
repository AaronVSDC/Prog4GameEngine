#ifndef SCORE_DISPLAY_COMPONENT_H
#define SCORE_DISPLAY_COMPONENT_H
#include <UndyneEngine.h>

//std
#include <vector>

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
		void refresh();

		UndyneEngine::TextComponent* m_Text{ nullptr };
		std::vector<UndyneEngine::Subject*> m_Subjects;
	};
}
#endif
