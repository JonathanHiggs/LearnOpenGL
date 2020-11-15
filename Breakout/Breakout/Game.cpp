#include <Breakout/Game.hpp>

#include <GLFW/glfw3.h>

#include <Breakout/SpriteRenderer.hpp>
#include <Breakout/ResourceManager.hpp>


namespace Breakout
{

    Game::Game(unsigned int width, unsigned int height)
        : State(GameState::Active)
        , Keys()
        , Width(width)
        , Height(height)
        , Level(0u)
        , Levels()
        , Player(nullptr)
        , Renderer(nullptr)
    { }

    Game::~Game()
    {
        if (Renderer != nullptr)
            delete Renderer;

        if (Player != nullptr)
            delete Player;
    }

    void Game::Init()
    {
        auto shader = ResourceManager::LoadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", nullptr, "sprite");

        glm::mat4 projection = glm::ortho(0.0f, (float)Width, (float)Height, 0.0f, -1.0f, 1.0f);

        shader.Use().SetInteger("image", 0);
        shader.SetMatrix4("projection", projection);

        ResourceManager::LoadTexture("assets/textures/background.jpg", false, "background");
        ResourceManager::LoadTexture("assets/textures/awesomeface.png", true, "face");
        ResourceManager::LoadTexture("assets/textures/block.png", false, "block");
        ResourceManager::LoadTexture("assets/textures/block_solid.png", false, "block_solid");
        ResourceManager::LoadTexture("assets/textures/paddle.png", true, "paddle");

        Levels.push_back(GameLevel::LoadLevel("assets/levels/one.lvl", this->Width, this->Height / 2));
        //Levels.push_back(GameLevel::LoadLevel("assets/levels/two.lvl", this->Width, this->Height / 2));
        //Levels.push_back(GameLevel::LoadLevel("assets/levels/three.lvl", this->Width, this->Height / 2));
        //Levels.push_back(GameLevel::LoadLevel("assets/levels/four.lvl", this->Width, this->Height / 2));

        auto playerPos = glm::vec2(
            Width / 2.0f - PLAYER_SIZE.x / 2.0f,
            Height - PLAYER_SIZE.y);
        auto paddle = ResourceManager::GetTexture("paddle");
        Player = new GameObject(playerPos, PLAYER_SIZE, paddle);

        Renderer = new SpriteRenderer(shader);
    }

    void Game::Update(float dt)
    {

    }

    void Game::ProcessInput(float dt)
    {
        if (State == GameState::Active)
        {
            auto velocity = PLAYER_VELOCITY * dt;

            if (Keys[GLFW_KEY_A] && Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;

            if (Keys[GLFW_KEY_D] && Player->Position.x <= Width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }

    void Game::Render()
    {
        if (State == GameState::Active)
        {
            auto background = ResourceManager::GetTexture("background");
            Renderer->DrawSprite(
                background, glm::vec2(0.0f, 0.0f), glm::vec2((float)Width, (float)Height));

            Levels[Level].Draw(*Renderer);
            Player->Draw(*Renderer);
        }
    }

}