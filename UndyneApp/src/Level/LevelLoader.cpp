#include "LevelLoader.h"
#include "../Components/DigField.h"

#include <SDL3/SDL.h>

//std
#include <fstream>
#include <memory>
#include <algorithm>

namespace Digger
{

	void LevelLoader::loadLevel(int levelIndex, UndyneEngine::Scene& scene)
	{

        std::vector<std::string> levelRows;
        if (!readLevelGrid(levelIndex, levelRows))
            return;

        int outputWidth = 0;
        int outputHeight = 0;
        SDL_GetRenderOutputSize(UndyneEngine::Renderer::getSDLRenderer(), &outputWidth, &outputHeight); 

        // Fit the native field into the window with an integer pixel scale and centre it.
        const float nativeFieldWidth = m_GridCollumns * m_NativeCellSize;   // 300
        const float nativeFieldHeight = m_GridRows * m_NativeCellSize;     // 200
        const float pixelScale = std::max(1.0f, std::floor(std::min(
            outputWidth / nativeFieldWidth, outputHeight / nativeFieldHeight)));
        const float originX = (outputWidth - nativeFieldWidth * pixelScale) * 0.5f;
        const float originY = (outputHeight - nativeFieldHeight * pixelScale) * 0.5f;

        const int rowCount = std::min(m_GridRows, static_cast<int>(levelRows.size()));

        // The field owns the grid geometry and bakes the earth background into its own
        // canvas, so it draws the whole playfield. Added before the entities and the
        // player so it renders underneath them.
        {
            auto field = std::make_unique<UndyneEngine::GameObject>("Field");
            m_DigField = field->addComponent<DigField>(
                m_GridCollumns, m_GridRows, m_NativeCellSize, pixelScale, originX, originY);
            scene.add(std::move(field));
        }

        // Entities and pre-dug tunnels on top of the field.
        for (int row = 0; row < rowCount; ++row)
        {
            const std::string& rowText = levelRows[row];
            const int columnCount = std::min(m_GridCollumns, static_cast<int>(rowText.size()));
            for (int column = 0; column < columnCount; ++column)
            {
                switch (rowText[column])
                {
                case '1': m_DigField->markDug({ column, row }); break;
                case '2': addEntityTile(column, row, scene, "Sprites/Gem.png");  break;
                case '3': addEntityTile(column, row, scene, "Sprites/GoldBag.BMP"); break;
                case '4': m_DigField->markDug({ column, row }); break;
                case '5':
                    m_DigField->markDug({ column, row });
                    m_StartCell = { column, row };
                    break;
                default: break;
                }
            }
        }
    }

    void LevelLoader::addEntityTile(int column, int row, UndyneEngine::Scene& scene, const std::string& texturePath) const
    {
        auto tile = std::make_unique<UndyneEngine::GameObject>("EntityTile");

        // Position and scale come straight from the field, so there is one source of truth.
        const glm::vec2 topLeft = m_DigField->cellTopLeft(column, row);
        tile->getTransform().setLocalPosition(topLeft.x, topLeft.y, 0.0f);

        auto* texture = tile->addComponent<UndyneEngine::TextureComponent>(texturePath);
        texture->setScale(m_DigField->pixelScale());

        scene.add(std::move(tile));
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