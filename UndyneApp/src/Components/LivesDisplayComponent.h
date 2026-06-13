#ifndef LIVES_DISPLAY_COMPONENT_H
#define LIVES_DISPLAY_COMPONENT_H
#include <UndyneEngine.h>
#include <memory>

namespace Digger
{
	class LivesDisplayComponent final
		: public UndyneEngine::BaseComponent
		, public UndyneEngine::Observer
	{
	public:
		~LivesDisplayComponent() override;

		void start() override;
		void render() const override;

		void onNotify(UndyneEngine::GameObject& subject, UndyneEngine::Event event) override;
		void onSubjectDestroyed(UndyneEngine::Subject& subject) override;

	private:
		int m_Lives{ 0 };
		UndyneEngine::Subject* m_LivesSubject{ nullptr };
		UndyneEngine::Texture2D* m_Icon{ nullptr };
	};
}
#endif
