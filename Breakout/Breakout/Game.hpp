#pragma once

#include <vector>

#include <irrKlang.h>

#include <Breakout/BallObject.hpp>
#include <Breakout/Collisions.hpp>
#include <Breakout/GameLevel.hpp>
#include <Breakout/ParticleGenerator.hpp>
#include <Breakout/PostProcessor.hpp>
#include <Breakout/PowerUp.hpp>
#include <Breakout/TextRenderer.hpp>
#include <Breakout/SpriteRenderer.hpp>


namespace Breakout
{

    enum class GameState
    {
        Active,
        Pause,
        Menu,
        LevelWin,
        Win
    };

    const glm::vec2 PLAYER_SIZE(150.0f, 20.0f);
    const float PLAYER_VELOCITY(900.0f);

    class Game
    {
    public:
        GameState State;
        bool Keys[1024];
        bool KeysProcessed[1024];
        unsigned int Width, Height;
        unsigned int Level;
        unsigned int Lives;
        std::vector<GameLevel> Levels;
        GameObject Player;
        BallObject Ball;
        ParticleGenerator Particles;
        std::vector<PowerUp> PowerUps;
        PostProcessor Effects;
        SpriteRenderer Renderer;
        TextRenderer Text;
        irrklang::ISoundEngine* SoundEngine;
        float ShakeTime = 0.0f;

    protected:
        Game(
            unsigned int width,
            unsigned int height,
            std::vector<GameLevel> levels,
            GameObject player,
            BallObject ball,
            ParticleGenerator particles,
            PostProcessor effects,
            SpriteRenderer renderer);

    public:
        static Game Create(unsigned int width, unsigned int height);
        ~Game();

        void Init();

        void ProcessInput(float deltaTime);

        void Update(float deltaTime);
        void CheckCollisions();

        void SpawnPowerUp(GameObject& block);
        bool IsOtherPowerUpActive(PowerUpType type);
        void ActivatePowerUp(PowerUp& powerUp);
        void UpdatePowerUps(float deltaTime);

        void Render();

        void ResetLevel();
        void ResetPlayer();
    };

}