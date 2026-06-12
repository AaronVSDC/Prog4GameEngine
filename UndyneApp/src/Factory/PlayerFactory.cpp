#include "PlayerFactory.h"
#include "../Components/MoveComponent.h"
#include "../Components/DigComponent.h"
#include "../Components/ScoreComponent.h"
#include "../Components/LivesComponent.h"
#include "../Components/ShootComponent.h"
#include <UndyneEngine.h>

using namespace UndyneEngine;

namespace Digger
{
	std::unique_ptr<GameObject> PlayerFactory::createPlayer(
		const std::string& name, glm::vec2 worldPosition, float cellSize, bool primary)
	{
		constexpr int columnCount = 4;
		constexpr float fillRatio = 0.7f;

		auto player = std::make_unique<GameObject>(name);
		TextureComponent* texture = player->addComponent<TextureComponent>("Sprites/PlayerSprites.png");
		player->addComponent<AnimationComponent>(columnCount);
		texture->setCentered(true);

		const float frameWidth = texture->getTextureSize().x / static_cast<float>(columnCount);
		if (frameWidth > 0.0f)
			texture->setScale(cellSize * fillRatio / frameWidth);

		player->getTransform().setLocalPosition(worldPosition.x, worldPosition.y, 0.0f);

		player->addComponent<MoveComponent>();
		player->addComponent<DigComponent>();
		player->addComponent<ShootComponent>();
		player->addComponent<ScoreComponent>();
		if (primary)
			player->addComponent<LivesComponent>();

		return player;
	}
}
