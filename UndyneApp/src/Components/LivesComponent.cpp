#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "MoveComponent.h"
#include "DigTrailComponent.h"

using namespace UndyneEngine;

namespace Digger
{
	LivesComponent::~LivesComponent()
	{
		if (m_Score) m_Score->removeObserver(this);
	}

	void LivesComponent::start()
	{
		if (ScoreComponent* score = getOwner()->getComponent<ScoreComponent>())
		{
			m_Score = score;
			score->addObserver(this);
		}
		notify(*getOwner(), Event::LivesChanged);
	}

	void LivesComponent::update(float)
	{
		if (not m_Dying) return;
		if (m_DeathAnimation and not m_DeathAnimation->isFinished()) return;

		m_Dying = false;
		m_DeathAnimation = nullptr;
		showGravestone(false);

		if (TextureComponent* texture = getOwner()->getComponent<TextureComponent>())
			texture->setVisible(true);
		if (MoveComponent* move = getOwner()->getComponent<MoveComponent>())
		{
			move->respawn();
			move->setEnabled(true);
		}

		if (Scene* scene = getOwner()->getScene())
			if (GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
				if (DigTrailComponent* trail = gridObject->getComponent<DigTrailComponent>())
					trail->resetStamp();
	}

	void LivesComponent::die()
	{
		if (m_GameOver or m_Dying) return;

		--m_Lives;
		notify(*getOwner(), Event::LivesChanged);

		if (TextureComponent* texture = getOwner()->getComponent<TextureComponent>())
			texture->setVisible(false);
		if (MoveComponent* move = getOwner()->getComponent<MoveComponent>())
			move->setEnabled(false);
		showGravestone(true);

		m_DeathAnimation = nullptr;
		if (Scene* scene = getOwner()->getScene())
			if (GameObject* grave = scene->findGameObjectByName("Gravestone"))
				if (AnimationComponent* animation = grave->getComponent<AnimationComponent>())
				{
					m_DeathAnimation = animation;
					animation->play();
				}

		if (m_Lives > 0)
		{
			m_Dying = true;
			notify(*getOwner(), Event::PlayerDied);
			return;
		}

		m_GameOver = true;
		if (Scene* scene = getOwner()->getScene())
			if (GameObject* textObject = scene->findGameObjectByName("GameOverText"))
				if (TextComponent* text = textObject->getComponent<TextComponent>())
					text->setText("GAME OVER");
	}

	void LivesComponent::showGravestone(bool visible)
	{
		Scene* scene = getOwner()->getScene();
		if (not scene) return;

		GameObject* grave = scene->findGameObjectByName("Gravestone");
		if (not grave) return;

		if (visible)
		{
			const glm::vec3 position = getOwner()->getTransform().getLocalPosition();
			grave->getTransform().setLocalPosition(position.x, position.y, 0.0f);
		}
		if (TextureComponent* texture = grave->getComponent<TextureComponent>())
			texture->setVisible(visible);
	}

	void LivesComponent::onNotify(GameObject& subject, Event event)
	{
		if (event != Event::ScoreChanged) return;
		ScoreComponent* score = subject.getComponent<ScoreComponent>();
		if (not score) return;
		while (score->getScore() >= m_NextExtraLife)
		{
			++m_Lives;
			m_NextExtraLife += 20000;
			notify(*getOwner(), Event::LivesChanged);
		}
	}

	void LivesComponent::onSubjectDestroyed(Subject& subject)
	{
		if (m_Score == &subject) m_Score = nullptr;
	}
}
