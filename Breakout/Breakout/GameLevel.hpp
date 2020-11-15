#pragma once

#include <vector>

#include <Breakout/GameObject.hpp>
#include <Breakout/SpriteRenderer.hpp>


namespace Breakout
{

    class GameLevel
    {
    public:
        std::vector<GameObject> Bricks;

        GameLevel() { }

        static GameLevel LoadLevel(const char* file, unsigned int levelWidth, unsigned int levelHeight);

        void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);

        void Draw(SpriteRenderer& renderer);

        bool IsComplete();

    private:
        void Init(
            std::vector<std::vector<unsigned int>> tileData,
            unsigned int levelWidth,
            unsigned int levelHeight);
    };

}