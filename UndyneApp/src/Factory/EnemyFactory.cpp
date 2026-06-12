#include "EnemyFactory.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/DigComponent.h"
#include "../Components/EnemyComponent.h"
#include "../AI/NobbinChaseState.h"
#include "../AI/ManualControlState.h"
#include <UndyneEngine.h>

using namespace UndyneEngine;

namespace Digger
{
	std::unique_ptr<GameObject> EnemyFactory::createMonster(
		const std::string& name, glm::vec2 worldPosition, float cellSize, bool manual)
	{
		constexpr int columnCount = 4;
		constexpr float fillRatio = 0.7f;

		auto monster = std::make_unique<GameObject>(name);
		TextureComponent* texture = monster->addComponent<TextureComponent>("Sprites/NobbinSprites.png");
		monster->addComponent<AnimationComponent>(columnCount);
		texture->setCentered(true);

		const float frameWidth = texture->getTextureSize().x / static_cast<float>(columnCount);
		if (frameWidth > 0.0f)
			texture->setScale(cellSize * fillRatio / frameWidth);

		monster->getTransform().setLocalPosition(worldPosition.x, worldPosition.y, 0.0f);

		monster->addComponent<GridMovementComponent>();
		DigComponent* dig = monster->addComponent<DigComponent>();
		dig->setAutoDig(false);
		dig->setClearsObstacles(true);

		EnemyComponent* enemy = monster->addComponent<EnemyComponent>();
		if (manual)
		{
			enemy->markManualControlled();
			monster->addComponent<StateMachineComponent>(std::make_unique<ManualControlState>());
		}
		else
		{
			monster->addComponent<StateMachineComponent>(std::make_unique<NobbinChaseState>());
		}

		return monster;
	}
}
