#include "LevelLoader.h"


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

        const float nativeFieldWidth = m_GridCollumns * m_NativeCellSize;   // 300
        const float nativeFieldHeight = m_GridRows * m_NativeCellSize;     // 200
        m_PixelScale = std::max(1.0f, std::floor(std::min(
            outputWidth / nativeFieldWidth, outputHeight / nativeFieldHeight)));
        m_OriginX = (outputWidth - nativeFieldWidth * m_PixelScale) * 0.5f;
        m_OriginY = (outputHeight - nativeFieldHeight * m_PixelScale) * 0.5f;

        const int rowCount = std::min(m_GridRows, static_cast<int>(levelRows.size()));

        // Pass 1 — background fills every cell.
        for (int row = 0; row < rowCount; ++row)
        {
            const std::string& rowText = levelRows[row];
            const int columnCount = std::min(m_GridCollumns, static_cast<int>(rowText.size()));
            for (int column = 0; column < columnCount; ++column)
                addBackgroundTile(column, row, scene);
        }

        // Pass 2 — entities drawn on top.
        for (int row = 0; row < rowCount; ++row)
        {
            const std::string& rowText = levelRows[row];
            const int columnCount = std::min(m_GridCollumns, static_cast<int>(rowText.size()));
            for (int column = 0; column < columnCount; ++column)
            {
                switch (rowText[column])
                {
                case '1': addEntityTile(column, row, scene, "Sprites/Hole/StartHole.png"); break;
                case '2': addEntityTile(column, row, scene, "Sprites/Gem.png");  break;
                case '3': addEntityTile(column, row, scene, "Sprites/GoldBag.BMP"); break;
                default: break;
                }
            }
        }
    }

    void LevelLoader::addBackgroundTile(int column, int row, UndyneEngine::Scene& scene) const
    {
        auto tile = std::make_unique<UndyneEngine::GameObject>("BackgroundTile");

        const float cellStride = m_NativeCellSize * m_PixelScale;
        tile->getTransform().setLocalPosition(
            m_OriginX + column * cellStride,
            m_OriginY + row * cellStride,
            0.0f);

        auto* tileTexture = tile->addComponent<UndyneEngine::TextureComponent>("Sprites/BackgroundLevel1.png");
        tileTexture->setScale(m_PixelScale);

        scene.add(std::move(tile));
    }

    void LevelLoader::addEntityTile(int column, int row, UndyneEngine::Scene& scene, const std::string& texturePath) const
    {
        auto tile = std::make_unique<UndyneEngine::GameObject>("EntityTile");

        const float cellStride = m_NativeCellSize * m_PixelScale;
        tile->getTransform().setLocalPosition(
            m_OriginX + column * cellStride,
            m_OriginY + row * cellStride,
            0.0f);

        auto* texture = tile->addComponent<UndyneEngine::TextureComponent>(texturePath);
        texture->setScale(m_PixelScale);

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