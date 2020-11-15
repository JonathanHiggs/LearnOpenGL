#pragma once

#include <vector>

#include <Breakout/BallObject.hpp>
#include <Breakout/Collisions.hpp>
#include <Breakout/GameLevel.hpp>
#include <Breakout/SpriteRenderer.hpp>


namespace Breakout
{

    enum class GameState
    {
        Active,
        Pause,
        Menu,
        Win
    };

    const glm::vec2 PLAYER_SIZE(150.0f, 20.0f);
    const float PLAYER_VELOCITY(900.0f);

    class Game
    {
    public:
        GameState State;
        bool Keys[2014];
        unsigned int Width, Height, Level;
        std::vector<GameLevel> Levels;
        GameObject Player;
        BallObject Ball;
        SpriteRenderer Renderer;

    protected:
        Game(
            unsigned int width,
            unsigned int height,
            std::vector<GameLevel> levels,
            GameObject player,
            BallObject ball,
            SpriteRenderer renderer);

    public:
        static Game Init(unsigned int width, unsigned int height);
        ~Game();

        void ProcessInput(float deltaTime);
        void Update(float deltaTime);
        void CheckCollisions();
        void Render();

        void ResetLevel();
        void ResetPlayer();
    };

}