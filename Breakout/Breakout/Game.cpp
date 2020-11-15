#include <Breakout/Game.hpp>

#include <GLFW/glfw3.h>

#include <Breakout/SpriteRenderer.hpp>
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

    void Game::Update(float dt)
    {
        Ball.Move(dt, Width);
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

    void Game::Render()
    {
        if (State == GameState::Active)
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

}