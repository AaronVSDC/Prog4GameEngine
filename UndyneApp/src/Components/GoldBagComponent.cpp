#include "GoldBagComponent.h"
#include "LevelGridComponent.h"
#include "ScoreComponent.h"
#include "LivesComponent.h"

#include <cmath>

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
			if (not isSupported())
				enterWobbling();
			break;
		case State::Wobbling:
		{
			if (isSupported())
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
		case State::Gold:
			updateGold();
			break;
		}
	}

	bool GoldBagComponent::isSupported() const
	{
		const glm::ivec2 below{ m_Cell.x, m_Cell.y + 1 };
		if (not m_Grid->inBounds(below)) return true;
		if (not m_Grid->isDug(below)) return true;
		if (m_Grid->objectAt(below) != nullptr) return true;
		if (playerOccupies(below)) return true;
		return false;
	}

	bool GoldBagComponent::playerOccupies(glm::ivec2 cell) const
	{
		if (not m_Player) return false;
		const glm::vec3 position = m_Player->getTransform().getLocalPosition();
		const glm::ivec2 playerCell = m_Grid->worldToCell({ position.x, position.y });
		return playerCell.x == cell.x and playerCell.y == cell.y;
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
		if (m_Player)
		{
			const glm::vec3 playerPosition = m_Player->getTransform().getLocalPosition();
			const glm::ivec2 playerCell = m_Grid->worldToCell({ playerPosition.x, playerPosition.y });
			if (playerCell.x == m_Cell.x and playerCell.y == m_Cell.y)
			{
				if (ScoreComponent* score = m_Player->getComponent<ScoreComponent>())
					score->collectGold();
				m_Grid->clearObjectAt(m_Cell);
				getOwner()->markForRemoval();
				return;
			}
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
		const glm::vec3 playerPosition = m_Player->getTransform().getLocalPosition();
		const glm::ivec2 playerCell = m_Grid->worldToCell({ playerPosition.x, playerPosition.y });
		if (playerCell.x == m_Cell.x and playerCell.y == m_Cell.y)
			if (LivesComponent* lives = m_Player->getComponent<LivesComponent>())
				lives->die();
	}

	void GoldBagComponent::pushTo(glm::ivec2 cell)
	{
		if (m_State != State::Resting) return;
		m_Grid->clearObjectAt(m_Cell);
		m_Cell = cell;
		m_Grid->setObjectAt(m_Cell, getOwner());
		setCellPosition();
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
