#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "MoveComponent.h"
#include "DigTrailComponent.h"
#include "../GameState/GameState.h"

using namespace UndyneEngine;

namespace Digger
{
	LivesComponent::~LivesComponent()
	{
		if (m_Score) m_Score->removeObserver(this);
	}

	void LivesComponent::start()
	{
		auto& soundSystem = SoundServiceLocator::getSoundSystem();
		soundSystem.loadSound("Audio/die.wav", "die");
		soundSystem.loadSound("Audio/dietune.wav", "dietune");
		soundSystem.loadSound("Audio/1up.wav", "1up");

		if (ScoreComponent* score = getOwner()->getComponent<ScoreComponent>())
		{
			m_Score = score;
			score->addObserver(this);
		}
		notify(*getOwner(), Event::LivesChanged);
	}

	void LivesComponent::update(float deltaTime)
	{
		if (m_DeathPending)
		{
			m_DeathTimer -= deltaTime;
			if (m_DeathTimer <= 0.0f)
			{
				m_DeathPending = false;
				beginDeath();
			}
			return;
		}

		if (!m_Dying) return;
		if (m_DeathAnimation and !m_DeathAnimation->isFinished()) return;

		m_Dying = false;
		m_DeathAnimation = nullptr;
		showGravestone(false);
		respawn();
		m_DyingPlayer = nullptr;
		GameState::resumeAction();
	}

	void LivesComponent::die(GameObject& dyingPlayer)
	{
		if (m_GameOver or m_DeathPending or m_Dying) return;

		SoundServiceLocator::getSoundSystem().playSound("die");

		m_DyingPlayer = &dyingPlayer;
		if (MoveComponent* move = dyingPlayer.getComponent<MoveComponent>())
			move->setEnabled(false);

		m_DeathPending = true;
		m_DeathTimer = s_DeathDelay;
		GameState::pauseAction();
		notify(*getOwner(), Event::PlayerDied);
	}

	void LivesComponent::beginDeath()
	{
		--m_Lives;
		notify(*getOwner(), Event::LivesChanged);

		if (m_DyingPlayer)
			if (TextureComponent* texture = m_DyingPlayer->getComponent<TextureComponent>())
				texture->setVisible(false);
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
			return;
		}

		m_GameOver = true;
		SoundServiceLocator::getSoundSystem().playSound("dietune");
		if (Scene* scene = getOwner()->getScene())
			if (GameObject* textObject = scene->findGameObjectByName("GameOverText"))
				if (TextComponent* text = textObject->getComponent<TextComponent>())
					text->setText("GAME OVER");
	}

	void LivesComponent::respawn()
	{
		if (!m_DyingPlayer) return;

		if (TextureComponent* texture = m_DyingPlayer->getComponent<TextureComponent>())
			texture->setVisible(true);
		if (MoveComponent* move = m_DyingPlayer->getComponent<MoveComponent>())
		{
			move->respawn();
			move->setEnabled(true);
		}

		if (Scene* scene = getOwner()->getScene())
			if (GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
				if (DigTrailComponent* trail = gridObject->getComponent<DigTrailComponent>())
					trail->resetStamp();
	}

	void LivesComponent::showGravestone(bool visible)
	{
		Scene* scene = getOwner()->getScene();
		if (!scene) return;

		GameObject* grave = scene->findGameObjectByName("Gravestone");
		if (!grave) return;

		if (visible and m_DyingPlayer)
		{
			const glm::vec3 position = m_DyingPlayer->getTransform().getLocalPosition();
			grave->getTransform().setLocalPosition(position.x, position.y, 0.0f);
		}
		if (TextureComponent* texture = grave->getComponent<TextureComponent>())
			texture->setVisible(visible);
	}

	void LivesComponent::onNotify(GameObject& subject, Event event)
	{
		if (event != Event::ScoreChanged) return;
		ScoreComponent* score = subject.getComponent<ScoreComponent>();
		if (!score) return;
		while (score->getScore() >= m_NextExtraLife)
		{
			++m_Lives;
			m_NextExtraLife += 20000;
			SoundServiceLocator::getSoundSystem().playSound("1up");
			notify(*getOwner(), Event::LivesChanged);
		}
	}

	void LivesComponent::onSubjectDestroyed(Subject& subject)
	{
		if (m_Score == &subject) m_Score = nullptr;
	}
}
