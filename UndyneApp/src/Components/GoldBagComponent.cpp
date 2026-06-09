#include "GoldBagComponent.h"
#include "LevelGridComponent.h"
#include "MoveComponent.h"
#include "ScoreComponent.h"
#include "LivesComponent.h"
#include "../AI/DeadState.h"

#include <cmath>
#include <vector>
#include <memory>

using namespace UndyneEngine;

namespace Digger
{
	GoldBagComponent::GoldBagComponent(glm::ivec2 cell) noexcept
		: m_Cell{ cell }
	{
	}

	void GoldBagComponent::start()
	{
		m_Texture = getOwner()->getComponent<TextureComponent>();

		if (Scene* scene = getOwner()->getScene())
		{
			m_Player = scene->findGameObjectByName("Player");
			if (GameObject* gridObject = scene->findGameObjectByName("LevelGrid"))
				m_Grid = gridObject->getComponent<LevelGridComponent>();
		}

		if (not m_Grid) return;

		m_FallSpeed = m_Grid->cellSize() * 8.0f;
		m_SlideSpeed = m_FallSpeed;
		if (m_Player)
			if (MoveComponent* move = m_Player->getComponent<MoveComponent>())
				m_SlideSpeed = move->speed();
		m_Grid->setObjectAt(m_Cell, getOwner());
		fitTextureToCell();
		setCellPosition();
	}

	void GoldBagComponent::update(float deltaTime)
	{
		if (not m_Grid) return;

		switch (m_State)
		{
		case State::Resting:
			if (not isHeld())
				enterWobbling();
			break;
		case State::Wobbling:
		{
			if (isHeld())
			{
				m_State = State::Resting;
				setCellPosition();
				break;
			}
			m_WobbleTimer -= deltaTime;
			const float elapsed = m_WobbleDuration - m_WobbleTimer;
			const float shake = std::sin(elapsed * 30.0f) * m_Grid->cellSize() * 0.06f;
			getOwner()->getTransform().setLocalPosition(
				m_Grid->laneCenterX(m_Cell.x) + shake, m_Grid->laneCenterY(m_Cell.y), 0.0f);
			if (m_WobbleTimer <= 0.0f)
				enterFalling();
			break;
		}
		case State::Falling:
			updateFalling(deltaTime);
			break;
		case State::Sliding:
			updateSliding(deltaTime);
			break;
		case State::Gold:
			updateGold();
			break;
		}

		m_PushedFromBelow = false;
	}

	bool GoldBagComponent::isSupported() const
	{
		const glm::ivec2 below{ m_Cell.x, m_Cell.y + 1 };
		if (not m_Grid->inBounds(below)) return true;
		if (not m_Grid->isDug(below)) return true;
		if (m_Grid->objectAt(below) != nullptr) return true;
		return false;
	}

	bool GoldBagComponent::isHeld() const
	{
		return isSupported() or m_PushedFromBelow;
	}

	void GoldBagComponent::enterWobbling()
	{
		m_State = State::Wobbling;
		m_WobbleTimer = m_WobbleDuration;
	}

	void GoldBagComponent::enterFalling()
	{
		m_Grid->clearObjectAt(m_Cell);
		m_State = State::Falling;
		m_RowsFallen = 0;
		m_FallY = m_Grid->laneCenterY(m_Cell.y);
	}

	void GoldBagComponent::updateFalling(float deltaTime)
	{
		m_FallY += m_FallSpeed * deltaTime;

		const glm::ivec2 cellNow = m_Grid->worldToCell({ m_Grid->laneCenterX(m_Cell.x), m_FallY });
		if (cellNow.y > m_Cell.y)
		{
			m_RowsFallen += cellNow.y - m_Cell.y;
			m_Cell.y = cellNow.y;
		}

		squashIfPlayerHit();
		squashMonster();

		const glm::ivec2 below{ m_Cell.x, m_Cell.y + 1 };
		const bool belowSolid = not m_Grid->inBounds(below) or not m_Grid->isDug(below) or m_Grid->objectAt(below) != nullptr;
		const float restY = m_Grid->laneCenterY(m_Cell.y);

		if (belowSolid and m_FallY >= restY)
		{
			m_FallY = restY;
			getOwner()->getTransform().setLocalPosition(m_Grid->laneCenterX(m_Cell.x), m_FallY, 0.0f);
			land();
			return;
		}

		getOwner()->getTransform().setLocalPosition(m_Grid->laneCenterX(m_Cell.x), m_FallY, 0.0f);
	}

	void GoldBagComponent::land()
	{
		if (m_RowsFallen > 1)
		{
			becomeGold();
			return;
		}
		m_State = State::Resting;
		m_Grid->setObjectAt(m_Cell, getOwner());
	}

	void GoldBagComponent::becomeGold()
	{
		m_State = State::Gold;
		if (m_Texture)
		{
			m_Texture->setTexture("Sprites/Gold.png");
			m_Texture->setSourceRect({ 0.0f, 0.0f, 15.0f, 18.0f });
			m_Texture->setScale((m_Grid->cellSize() * 0.7f) / 15.0f);
		}
		m_Grid->setObjectAt(m_Cell, getOwner());
		setCellPosition();
	}

	void GoldBagComponent::updateGold()
	{
		if (m_Player and m_Grid->isOnCell(*m_Player, m_Cell))
		{
			if (ScoreComponent* score = m_Player->getComponent<ScoreComponent>())
				score->collectGold();
			m_Grid->clearObjectAt(m_Cell);
			getOwner()->markForRemoval();
			return;
		}

		const glm::ivec2 below{ m_Cell.x, m_Cell.y + 1 };
		if (m_Grid->inBounds(below) and m_Grid->isDug(below) and m_Grid->objectAt(below) == nullptr)
		{
			m_Grid->clearObjectAt(m_Cell);
			getOwner()->markForRemoval();
		}
	}

	void GoldBagComponent::squashIfPlayerHit()
	{
		if (not m_Player) return;
		if (m_Grid->isOnCell(*m_Player, m_Cell))
			if (LivesComponent* lives = m_Player->getComponent<LivesComponent>())
				lives->die();
	}

	void GoldBagComponent::squashMonster()
	{
		Scene* scene = getOwner()->getScene();
		if (not scene) return;

		GameObject* monster = scene->findGameObjectByName("Nobbin");
		if (not monster) return;

		if (m_Grid->isOnCell(*monster, m_Cell))
			if (StateMachineComponent* machine = monster->getComponent<StateMachineComponent>())
				machine->changeState(std::make_unique<DeadState>());
	}

	bool GoldBagComponent::tryPush(int directionX)
	{
		if (m_State != State::Resting) return false;

		std::vector<GoldBagComponent*> chain;
		glm::ivec2 cell = m_Cell;
		for (GoldBagComponent* bag = this; bag and bag->m_State == State::Resting; )
		{
			chain.push_back(bag);
			cell = { cell.x + directionX, cell.y };
			GameObject* occupant = m_Grid->objectAt(cell);
			bag = occupant ? occupant->getComponent<GoldBagComponent>() : nullptr;
		}

		if (not m_Grid->inBounds(cell)) return false;
		if (m_Grid->objectAt(cell) != nullptr) return false;

		for (auto bag = chain.rbegin(); bag != chain.rend(); ++bag)
			(*bag)->startSlide(directionX);
		return true;
	}

	void GoldBagComponent::startSlide(int directionX)
	{
		m_Grid->clearObjectAt(m_Cell);
		m_Cell = { m_Cell.x + directionX, m_Cell.y };
		m_Grid->setObjectAt(m_Cell, getOwner());
		m_SlideTargetX = m_Grid->laneCenterX(m_Cell.x);
		m_State = State::Sliding;
	}

	void GoldBagComponent::updateSliding(float deltaTime)
	{
		const glm::vec3 position = getOwner()->getTransform().getLocalPosition();
		const float laneY = m_Grid->laneCenterY(m_Cell.y);
		const float step = m_SlideSpeed * deltaTime;

		if (std::abs(m_SlideTargetX - position.x) <= step)
		{
			getOwner()->getTransform().setLocalPosition(m_SlideTargetX, laneY, 0.0f);
			m_State = State::Resting;
			if (not isSupported())
				enterFalling();
			return;
		}

		const float nextX = position.x + ((m_SlideTargetX > position.x) ? step : -step);
		getOwner()->getTransform().setLocalPosition(nextX, laneY, 0.0f);
	}

	void GoldBagComponent::fitTextureToCell()
	{
		if (not m_Texture) return;
		const glm::vec2 size = m_Texture->getTextureSize();
		if (size.x <= 0.0f) return;
		m_Texture->setScale((m_Grid->cellSize() * 0.7f) / size.x);
	}

	void GoldBagComponent::setCellPosition()
	{
		getOwner()->getTransform().setLocalPosition(
			m_Grid->laneCenterX(m_Cell.x), m_Grid->laneCenterY(m_Cell.y), 0.0f);
	}
}
