#include "LivesDisplayComponent.h"
#include "LivesComponent.h"

using namespace UndyneEngine;

namespace Digger
{
	LivesDisplayComponent::~LivesDisplayComponent()
	{
		if (m_LivesSubject) m_LivesSubject->removeObserver(this);
	}

	void LivesDisplayComponent::start()
	{
		m_Icon = ResourceManager::loadTexture("Sprites/Life.png");

		if (Scene* scene = getOwner()->getScene())
			if (GameObject* player = scene->findGameObjectByName("Player"))
				if (LivesComponent* lives = player->getComponent<LivesComponent>())
				{
					m_LivesSubject = lives;
					lives->addObserver(this);
					m_Lives = lives->getLives();
				}
	}

	void LivesDisplayComponent::onNotify(GameObject& subject, Event event)
	{
		if (event != Event::LivesChanged) return;
		if (LivesComponent* lives = subject.getComponent<LivesComponent>())
			m_Lives = lives->getLives();
	}

	void LivesDisplayComponent::onSubjectDestroyed(Subject& subject)
	{
		if (m_LivesSubject == &subject) m_LivesSubject = nullptr;
	}

	void LivesDisplayComponent::render() const
	{
		if (not m_Icon) return;
		const glm::vec3& position = getOwner()->getTransform().getWorldPosition();
		const float iconWidth = 32.0f;
		const float iconHeight = 30.0f;
		const float spacing = iconWidth + 6.0f;
		for (int index = 0; index < m_Lives; ++index)
			Renderer::renderTexture(*m_Icon, position.x + index * spacing, position.y, iconWidth, iconHeight);
	}
}
