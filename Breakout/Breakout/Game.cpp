#include <Breakout/Game.hpp>

#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/gtx/norm.hpp>

#include <Breakout/ResourceManager.hpp>


namespace Breakout
{

    Game::Game(
        unsigned int width,
        unsigned int height,
        std::vector<GameLevel> levels,
        GameObject player,
        BallObject ball,
        SpriteRenderer renderer
    )
        : State(GameState::Active)
        , Keys()
        , Width(width)
        , Height(height)
        , Level(0u)
        , Levels(levels)
        , Player(player)
        , Ball(ball)
        , Renderer(std::move(renderer))
    { }

    Game::~Game()
    { }

    Game Game::Init(unsigned int width, unsigned int height)
    {
        auto shader = ResourceManager::LoadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", nullptr, "sprite");
        glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
        shader.Use().SetInteger("image", 0);
        shader.SetMatrix4("projection", projection);

        ResourceManager::LoadTexture("assets/textures/background.jpg", false, "background");
        ResourceManager::LoadTexture("assets/textures/awesomeface.png", true, "face");
        ResourceManager::LoadTexture("assets/textures/block.png", false, "block");
        ResourceManager::LoadTexture("assets/textures/block_solid.png", false, "block_solid");
        ResourceManager::LoadTexture("assets/textures/paddle.png", true, "paddle");

        auto levels = std::vector<GameLevel>();
        levels.push_back(GameLevel::LoadLevel("assets/levels/one.lvl", width, height / 2));
        //levels.push_back(GameLevel::LoadLevel("assets/levels/two.lvl", width, height / 2));
        //levels.push_back(GameLevel::LoadLevel("assets/levels/three.lvl", width, height / 2));
        //levels.push_back(GameLevel::LoadLevel("assets/levels/four.lvl", width, height / 2));

        auto playerPos = glm::vec2(
            width / 2.0f - PLAYER_SIZE.x / 2.0f,
            height - PLAYER_SIZE.y);

        auto player = GameObject(
            playerPos,
            PLAYER_SIZE,
            ResourceManager::GetTexture("paddle"));

        auto ball = BallObject(
            playerPos + glm::vec2(
                PLAYER_SIZE.x / 2.0f - BallObject::InitialRadius,
                -BallObject::InitialRadius * 2.0f),
            BallObject::InitialRadius,
            BallObject::InitialVelocity,
            ResourceManager::GetTexture("face"));

        auto renderer = SpriteRenderer(shader);

        return Game(
            width, height,
            levels,
            player,
            ball,
            std::move(renderer));
    }

    void Game::ProcessInput(float dt)
    {
        if (State == GameState::Active)
        {
            auto velocity = PLAYER_VELOCITY * dt;

            if (Keys[GLFW_KEY_A] && Player.Position.x >= 0.0f)
            {
                Player.Position.x -= velocity;

                if (Ball.Stuck)
                    Ball.Position.x -= velocity;
            }

            if (Keys[GLFW_KEY_D] && Player.Position.x <= Width - Player.Size.x)
            {
                Player.Position.x += velocity;

                if (Ball.Stuck)
                    Ball.Position.x += velocity;
            }

            if (Keys[GLFW_KEY_SPACE])
                Ball.Stuck = false;
        }
    }

    void Game::Update(float dt)
    {
        if (State == GameState::Active)
        {
            Ball.Move(dt, Width);
            CheckCollisions();

            if (Ball.Position.y >= Height)
            {
                ResetLevel();
                ResetPlayer();
            }
        }
        else if (State == GameState::Pause)
        {
            dt = 1.0f / 60.0f;
            Ball.Move(dt, Width);
            CheckCollisions();
        }
    }

    void Game::CheckCollisions()
    {
        for (auto& brick : Levels[Level].Bricks)
        {
            if (!brick.Destroyed)
            {
                auto result = CheckCollision(Ball, brick);
                if (result.Collision)
                {
                    if (!brick.IsSolid)
                        brick.Destroyed = true;

                    if (result.Direction == Direction::Left || result.Direction == Direction::Right)
                    {
                        Ball.Velocity.x = -Ball.Velocity.x;
                        auto penitration = Ball.Radius - std::abs(result.Difference.x);
                        if (result.Direction == Direction::Left)
                            Ball.Position.x += penitration;
                        else
                            Ball.Position.x -= penitration;
                    }
                    else
                    {
                        Ball.Velocity.y = -Ball.Velocity.y;
                        auto penitration = Ball.Radius - std::abs(result.Difference.y);
                        if (result.Direction == Direction::Up)
                            Ball.Position.y -= penitration;
                        else
                            Ball.Position.y += penitration;
                    }
                }
            }
        }

        auto result = CheckCollision(Ball, Player);
        if (!Ball.Stuck && result.Collision)
        {
            float centerBoard = Player.Position.x + Player.Size.x / 2.0f;
            float distance = (Ball.Position.x + Ball.Radius) - centerBoard;
            float proportion = distance / (Player.Size.x / 2.0f);

            float strength = 2.0f;

            auto speed = glm::length(Ball.Velocity);

            Ball.Velocity =
                glm::normalize(
                    glm::vec2(
                        BallObject::InitialVelocity.x * proportion * strength,
                        -1.0f * abs(Ball.Velocity.y)))
                * speed;
        }
    }

    void Game::Render()
    {
        if (State == GameState::Active || State == GameState::Pause)
        {
            auto background = ResourceManager::GetTexture("background");
            Renderer.DrawSprite(
                background,
                glm::vec2(0.0f, 0.0f),
                glm::vec2((float)Width, (float)Height));

            Levels[Level].Draw(Renderer);
            Player.Draw(Renderer);
            Ball.Draw(Renderer);
        }
    }

    void Game::ResetLevel()
    {
        Levels[0].Load("assets/levels/one.lvl", Width, Height / 2);
    }

    void Game::ResetPlayer()
    {
        Player.Size = PLAYER_SIZE;
        Player.Position = glm::vec2(Width / 2.0f - PLAYER_SIZE.x / 2.0f, Height - PLAYER_SIZE.y);
        Ball.Reset(
            Player.Position + glm::vec2(
                PLAYER_SIZE.x / 2.0f - BallObject::InitialRadius,
                -(BallObject::InitialRadius * 2.0f)),
            BallObject::InitialVelocity);
    }

}