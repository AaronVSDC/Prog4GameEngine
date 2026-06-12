#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"

//std
#include <algorithm>
#include <string>

using namespace UndyneEngine;

namespace Digger
{
	ScoreDisplayComponent::~ScoreDisplayComponent()
	{
		for (Subject* subject : m_Subjects)
			subject->removeObserver(this);
	}

	void ScoreDisplayComponent::start()
	{
		m_Text = getOwner()->getComponent<TextComponent>();

		if (Scene* scene = getOwner()->getScene())
			for (GameObject* player : scene->findGameObjectsWithComponent<ScoreComponent>())
				if (ScoreComponent* score = player->getComponent<ScoreComponent>())
				{
					m_Subjects.push_back(score);
					score->addObserver(this);
				}

		refresh();
	}

	void ScoreDisplayComponent::onNotify(GameObject&, Event event)
	{
		if (event == Event::ScoreChanged)
			refresh();
	}

	void ScoreDisplayComponent::onSubjectDestroyed(Subject& subject)
	{
		std::erase(m_Subjects, &subject);
	}

	void ScoreDisplayComponent::refresh()
	{
		if (not m_Text)
			return;

		int total = 0;
		if (Scene* scene = getOwner()->getScene())
			for (GameObject* player : scene->findGameObjectsWithComponent<ScoreComponent>())
				if (ScoreComponent* score = player->getComponent<ScoreComponent>())
					total += score->getScore();

		m_Text->setText(std::to_string(total));
	}
}
