#include <Breakout/GameLevel.hpp>

#include <fstream>
#include <sstream>

#include <Breakout/ResourceManager.hpp>


namespace Breakout
{

    GameLevel GameLevel::LoadLevel(const char* file, unsigned int levelWidth, unsigned int levelHeight)
    {
        GameLevel level;
        level.Load(file, levelWidth, levelHeight);
        return level;
    }

    void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
    {
        this->Bricks.clear();

        unsigned int tileCode;
        GameLevel level;
        std::string line;
        std::ifstream fstream(file);

        std::vector<std::vector<unsigned int>> tileData;

        if (fstream)
        {
            while (std::getline(fstream, line))
            {
                std::istringstream sstream(line);
                std::vector<unsigned int> row;
                while (sstream >> tileCode)
                    row.push_back(tileCode);
                tileData.push_back(row);
            }

            if (tileData.size() > 0)
                this->Init(tileData, levelWidth, levelHeight);
        }
    }

    void GameLevel::Draw(SpriteRenderer& renderer)
    {
        for (auto& tile : Bricks)
            if (!tile.Destroyed)
                tile.Draw(renderer);
    }

    bool GameLevel::IsComplete()
    {
        for (auto& tile : Bricks)
            if (!tile.IsSolid && !tile.Destroyed)
                return false;

        return true;
    }

    void GameLevel::Init(
        std::vector<std::vector<unsigned int>> tileData,
        unsigned int levelWidth,
        unsigned int levelHeight)
    {
        auto height = tileData.size();
        auto width = tileData[0].size();
        auto unitWidth = levelWidth / (float)width;
        auto unitHeight = levelHeight / (float)height;

        for (auto y = 0u; y < height; y++)
        {
            for (auto x = 0u; x < width; x++)
            {
                Texture2D texture;
                glm::vec3 color;
                bool solid;

                auto value = tileData[y][x];
                if (value == 1) // solid blocks
                {
                    texture = ResourceManager::GetTexture("block_solid");
                    color = glm::vec3(0.8f, 0.8f, 0.7f);
                    solid = true;
                }
                else if (value > 1) // colored blocks
                {
                    texture = ResourceManager::GetTexture("block");
                    switch (value)
                    {
                    case 2:
                        color = glm::vec3(0.2f, 0.6f, 1.0f);
                        break;

                    case 3:
                        color = glm::vec3(0.0f, 0.7f, 0.0f);
                        break;

                    case 4:
                        color = glm::vec3(0.8f, 0.8f, 0.4f);
                        break;

                    case 5:
                        color = glm::vec3(1.0f, 0.5f, 0.0f);
                        break;

                    default:
                        color = glm::vec3(1.0f);
                        break;
                    }
                    solid = false;
                }
                else
                    continue;

                auto position = glm::vec2(unitWidth * x, unitHeight * y);
                auto velocity = glm::vec2(0.0f);
                auto size = glm::vec2(unitWidth, unitHeight);
                auto object = GameObject(position, velocity, size, texture, color, solid);
                this->Bricks.push_back(object);
            }
        }
    }

}