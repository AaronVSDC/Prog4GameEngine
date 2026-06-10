#include "LevelLoader.h"
#include "../Components/LevelGridComponent.h"
#include "../Components/DigTrailComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/DigComponent.h"
#include "../Components/PickupComponent.h"
#include "../Components/GoldBagComponent.h"
#include "../Components/ScoreComponent.h"
#include "../Components/ScoreDisplayComponent.h"
#include "../Components/LivesComponent.h"
#include "../Components/LivesDisplayComponent.h"
#include "../Components/ShootComponent.h"
#include "../Components/MonsterSpawnerComponent.h"
#include "../Components/LevelController.h"
#include <UndyneEngine.h>

//std
#include <fstream>
#include <algorithm>
#include <memory>

using namespace UndyneEngine;

namespace Digger
{
	void LevelLoader::load(int levelIndex, Scene& scene) const
	{
		LevelData level;
		if (!parse(levelIndex, level))
			return;

		auto gridObject = std::make_unique<GameObject>("LevelGrid");
		auto* levelGrid = gridObject->addComponent<LevelGridComponent>(level.columns, level.rows, level.nativeCellSize, level.dugCells);
		gridObject->addComponent<DigTrailComponent>(); 
		scene.add(std::move(gridObject));

		constexpr float fillRatio = 0.7f;

		auto player = std::make_unique<GameObject>("Player");

		const int playerColumnCount = 4;
		auto* playerTexture = player->addComponent<TextureComponent>("Sprites/PlayerSprites.png");
		player->addComponent<AnimationComponent>(playerColumnCount);
		playerTexture->setCentered(true);

		const float frameWidth = playerTexture->getTextureSize().x / playerColumnCount;
		playerTexture->setScale(levelGrid->cellSize() * fillRatio / frameWidth);

		const glm::vec2 startCenter{ levelGrid->laneCenterX(level.startCell.x), levelGrid->laneCenterY(level.startCell.y) };
		player->getTransform().setLocalPosition(startCenter.x, startCenter.y, 0.0f);
		player->addComponent<MoveComponent>();
		player->addComponent<DigComponent>();
		player->addComponent<ScoreComponent>();
		player->addComponent<LivesComponent>();
		player->addComponent<ShootComponent>();
		scene.add(std::move(player));

		for (const LevelData::Placement& placement : level.entities)
		{
			const glm::ivec2 cell{ placement.column, placement.row };
			const bool isGoldBag = placement.texture == "Sprites/GoldBag.BMP";

			auto entity = std::make_unique<GameObject>(isGoldBag ? "GoldBag" : "Emerald");
			auto* texture = entity->addComponent<TextureComponent>(placement.texture);
			texture->setCentered(true);
			texture->setScale((levelGrid->cellSize() * fillRatio) / texture->getTextureSize().x);
			entity->getTransform().setLocalPosition(
				levelGrid->laneCenterX(cell.x), levelGrid->laneCenterY(cell.y), 0.0f);

			if (isGoldBag)
				entity->addComponent<GoldBagComponent>(cell);
			else
				entity->addComponent<PickupComponent>(cell);

			scene.add(std::move(entity));
		}

		if (not level.spawnCells.empty())
		{
			const int cycleLevel = ((std::max(1, levelIndex) - 1) % 10) + 1;
			const int monsterTotal = cycleLevel + 5;
			const int monsterMaxAlive = (cycleLevel <= 1) ? 3 : (cycleLevel <= 7 ? 4 : 5);
			constexpr float secondsPerSpawnTick = 0.08f;
			const float monsterInterval = (45.0f - 2.0f * static_cast<float>(cycleLevel)) * secondsPerSpawnTick;

			auto spawner = std::make_unique<GameObject>("MonsterSpawner");
			auto* monsterSpawner = spawner->addComponent<MonsterSpawnerComponent>();
			monsterSpawner->configure(level.spawnCells.front(), monsterTotal, monsterMaxAlive, monsterInterval);
			scene.add(std::move(spawner));
		}

		auto gravestone = std::make_unique<GameObject>("Gravestone");
		auto* graveTexture = gravestone->addComponent<TextureComponent>("Sprites/GraveSprites.png");
		gravestone->addComponent<AnimationComponent>(5, 3.0f, false);
		graveTexture->setCentered(true);
		graveTexture->setScale((levelGrid->cellSize() * fillRatio) / 15.0f);
		graveTexture->setVisible(false);
		scene.add(std::move(gravestone));

		auto hud = std::make_unique<GameObject>("Hud");
		auto scoreFont = ResourceManager::loadFont("ScoreBoardFont.otf", 36);
		hud->addComponent<TextComponent>("0", scoreFont);
		hud->addComponent<ScoreDisplayComponent>();

		const glm::vec2 playfieldTopLeft = levelGrid->cellTopLeft(0, 0);
		hud->getTransform().setLocalPosition(playfieldTopLeft.x, playfieldTopLeft.y * 0.3f, 0.0f);

		scene.add(std::move(hud));

		auto livesHud = std::make_unique<GameObject>("LivesHud");
		livesHud->addComponent<LivesDisplayComponent>();
		const glm::vec2 fieldTopRight = levelGrid->cellTopLeft(level.columns, 0);
		livesHud->getTransform().setLocalPosition(fieldTopRight.x - 130.0f, playfieldTopLeft.y * 0.3f, 0.0f);
		scene.add(std::move(livesHud));

		auto gameOver = std::make_unique<GameObject>("GameOverText");
		auto gameOverFont = ResourceManager::loadFont("ScoreBoardFont.otf", 48);
		gameOver->addComponent<TextComponent>("", gameOverFont);
		gameOver->getTransform().setLocalPosition(
			levelGrid->laneCenterX(level.columns / 2) - 130.0f, levelGrid->laneCenterY(level.rows / 2), 0.0f);
		scene.add(std::move(gameOver));

		auto controller = std::make_unique<GameObject>("LevelController");
		controller->addComponent<LevelController>();
		scene.add(std::move(controller));
	}

	bool LevelLoader::parse(int levelIndex, LevelData& outData) const
	{
		std::vector<std::string> levelRows;
		if (!readLevelGrid(levelIndex, levelRows))
			return false;

		outData.columns = m_GridColumns;
		outData.rows = m_GridRows;
		outData.nativeCellSize = m_NativeCellSize;

		const int rowCount = std::min(m_GridRows, static_cast<int>(levelRows.size()));
		for (int row = 0; row < rowCount; ++row)
		{
			const std::string& rowText = levelRows[row];
			const int columnCount = std::min(m_GridColumns, static_cast<int>(rowText.size()));
			for (int column = 0; column < columnCount; ++column)
			{
				switch (rowText[column])
				{
				case '1':
					outData.dugCells.push_back({ column, row });
					break;
				case '4':
					outData.dugCells.push_back({ column, row });
					outData.spawnCells.push_back({ column, row });
					break;
				case '2':
					outData.entities.push_back({ column, row, "Sprites/Gem.png", 25 });
					break;
				case '3':
					outData.entities.push_back({ column, row, "Sprites/GoldBag.BMP", 0 });
					break;
				case '5':
					outData.dugCells.push_back({ column, row });
					outData.startCell = { column, row };
					break;
				default:
					break;
				}
			}
		}

		return true;
	}

	bool LevelLoader::readLevelGrid(int levelIndex, std::vector<std::string>& outRows) const
	{
		const auto fullPath = UndyneEngine::ResourceManager::getDataPath() / "Levels.json";
		std::ifstream levelFile(fullPath);
		if (!levelFile.is_open())
		{
			UDE_ERROR("LevelLoader: cannot open {}", fullPath.string());
			return false;
		}

		const std::string header = "Level: " + std::to_string(levelIndex);
		std::string line;
		bool headerFound = false;
		while (std::getline(levelFile, line))
		{
			if (line.rfind(header, 0) == 0)
			{
				headerFound = true;
				break;
			}
		}

		if (!headerFound)
		{
			UDE_ERROR("LevelLoader: level {} not found", levelIndex);
			return false;
		}

		outRows.clear();
		for (int row = 0; row < m_GridRows && std::getline(levelFile, line); ++row)
			outRows.push_back(line);

		return true;
	}
}
