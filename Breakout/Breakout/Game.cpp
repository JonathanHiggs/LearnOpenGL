#include <Breakout/Game.hpp>

#include <Breakout/SpriteRenderer.hpp>
#include <Breakout/ResourceManager.hpp>


namespace Breakout
{

    Game::Game(unsigned int width, unsigned int height)
        : State(GameState::Active)
        , Keys()
        , Width(width)
        , Height(height)
        , renderer(nullptr)
    {

    }

    Game::~Game()
    {
        if (renderer != nullptr)
            delete renderer;
    }

    void Game::Init()
    {
        auto shader = ResourceManager::LoadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", nullptr, "sprite");

        glm::mat4 projection = glm::ortho(
            0.0f,
            (float)(this->Width),
            (float)(this->Height),
            0.0f,
            -1.0f,
            1.0f);

        shader.Use().SetInteger("image", 0);
        shader.SetMatrix4("projection", projection);

        renderer = new SpriteRenderer(shader);

        ResourceManager::LoadTexture("assets/textures/awesomeface.png", true, "face");
    }

    void Game::Update(float dt)
    {

    }

    void Game::ProcessInput(float dt)
    {

    }

    void Game::Render()
    {
        auto face = ResourceManager::GetTexture("face");

        renderer->DrawSprite(
            face,
            glm::vec2(200.0f, 200.0f),
            glm::vec2(300.0f, 400.0f),
            45.0f,
            glm::vec3(0.0f, 1.0f, 0.0f));
    }

}