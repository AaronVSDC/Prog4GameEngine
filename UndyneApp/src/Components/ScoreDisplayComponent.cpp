#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"

#include <string>

namespace Digger
{
	ScoreDisplayComponent::~ScoreDisplayComponent()
	{
		if (m_Subject)
			m_Subject->removeObserver(this);
	}

	void ScoreDisplayComponent::start()
	{
		m_Text = getOwner()->getComponent<UndyneEngine::TextComponent>();

		if (UndyneEngine::Scene* scene = getOwner()->getScene())
			if (UndyneEngine::GameObject* player = scene->findGameObjectByName("Player"))
				if (ScoreComponent* score = player->getComponent<ScoreComponent>())
				{
					m_Subject = score;
					score->addObserver(this);
					if (m_Text)
						m_Text->setText("Score: " + std::to_string(score->getScore()));
				}
	}

	void ScoreDisplayComponent::onNotify(UndyneEngine::GameObject& subject, UndyneEngine::Event event)
	{
		if (event != UndyneEngine::Event::ScoreChanged or !m_Text)
			return;
		if (ScoreComponent* score = subject.getComponent<ScoreComponent>())
			m_Text->setText("Score: " + std::to_string(score->getScore()));
	}

	void ScoreDisplayComponent::onSubjectDestroyed(UndyneEngine::Subject& subject)
	{
		if (m_Subject == &subject)
			m_Subject = nullptr;
	}
}
