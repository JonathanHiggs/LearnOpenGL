#include <Breakout/Game.hpp>

#include <algorithm>
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
        ParticleGenerator particles,
        PostProcessor effects,
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
        , Particles(std::move(particles))
        , Effects(std::move(effects))
        , Renderer(std::move(renderer))
    {
        PowerUps.reserve(20u);
    }

    Game::~Game()
    { }

    Game Game::Init(unsigned int width, unsigned int height)
    {
        glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

        auto shader = ResourceManager::LoadShader(
            "assets/shaders/sprite.vert", "assets/shaders/sprite.frag", nullptr, "sprite");
        shader.Use().SetInteger("image", 0);
        shader.SetMatrix4("projection", projection);

        auto particleShader = ResourceManager::LoadShader(
            "assets/shaders/particle.vert", "assets/shaders/particle.frag", nullptr, "particle");
        particleShader.Use().SetInteger("sprite", 0);
        particleShader.SetMatrix4("projection", projection);

        auto postProcessorShader = ResourceManager::LoadShader(
            "assets/shaders/post-processing.vert", "assets/shaders/post-processing.frag", nullptr, "post-processor");

        ResourceManager::LoadTexture("assets/textures/background.jpg", false, "background");
        ResourceManager::LoadTexture("assets/textures/awesomeface.png", true, "face");
        ResourceManager::LoadTexture("assets/textures/block.png", false, "block");
        ResourceManager::LoadTexture("assets/textures/block_solid.png", false, "block_solid");
        ResourceManager::LoadTexture("assets/textures/paddle.png", true, "paddle");
        ResourceManager::LoadTexture("assets/textures/particle.png", true, "particle");

        ResourceManager::LoadTexture("assets/textures/powerup_chaos.png", true, "chaos");
        ResourceManager::LoadTexture("assets/textures/powerup_confuse.png", true, "confuse");
        ResourceManager::LoadTexture("assets/textures/powerup_increase.png", true, "increase");
        ResourceManager::LoadTexture("assets/textures/powerup_passthrough.png", true, "passthrough");
        ResourceManager::LoadTexture("assets/textures/powerup_speed.png", true, "speed");
        ResourceManager::LoadTexture("assets/textures/powerup_sticky.png", true, "sticky");

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
            glm::vec2(0.0f),
            PLAYER_SIZE,
            ResourceManager::GetTexture("paddle"));

        auto ball = BallObject(
            playerPos + glm::vec2(
                PLAYER_SIZE.x / 2.0f - BallObject::InitialRadius,
                -BallObject::InitialRadius * 2.0f),
            BallObject::InitialRadius,
            BallObject::InitialVelocity,
            ResourceManager::GetTexture("face"));

        auto particles = ParticleGenerator(
            particleShader,
            ResourceManager::GetTexture("particle"),
            500u);

        auto effects = PostProcessor(
            postProcessorShader,
            width, height);

        auto renderer = SpriteRenderer(shader);

        return Game(
            width, height,
            levels,
            player,
            ball,
            std::move(particles),
            std::move(effects),
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

            auto newParticles = Ball.Stuck ? 0 : 2;

            Particles.Update(dt, Ball, newParticles, glm::vec2(Ball.Radius / 2.0f));

            UpdatePowerUps(dt);

            if (ShakeTime > 0.0f)
            {
                ShakeTime -= dt;
                if (ShakeTime <= 0.0f)
                {
                    ShakeTime = 0.0f;
                    Effects.Shake = false;
                }
            }
        }
    }

    void Game::UpdatePowerUps(float dt)
    {
        for (auto& powerUp : PowerUps)
        {
            powerUp.Position += powerUp.Velocity * dt;

            if (powerUp.Activated)
            {
                powerUp.Duration -= dt;

                if (powerUp.Duration <= 0.0f)
                {
                    powerUp.Activated = false;

                    switch (powerUp.Type)
                    {
                    case PowerUpType::Chaos:
                    {
                        if (!IsOtherPowerUpActive(PowerUpType::Chaos))
                            Effects.Chaos = false;
                    } break;

                    case PowerUpType::Confuse:
                    {
                        if (!IsOtherPowerUpActive(PowerUpType::Confuse))
                            Effects.Confuse = false;
                    } break;

                    case PowerUpType::PassThrough:
                    {
                        if (!IsOtherPowerUpActive(PowerUpType::PassThrough))
                        {
                            Ball.PassThrough = false;
                            Ball.Color = glm::vec3(1.0f);
                        }
                    } break;

                    case PowerUpType::Sticky:
                    {
                        if (!IsOtherPowerUpActive(PowerUpType::Sticky))
                        {
                            Ball.Sticky = false;
                            Player.Color = glm::vec3(1.0f);
                        }
                    } break;

                    default:
                        break;
                    }
                }
            }
        }

        PowerUps.erase(
            std::remove_if(
                PowerUps.begin(),
                PowerUps.end(),
                [](PowerUp const& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }),
            PowerUps.end());
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
                    {
                        brick.Destroyed = true;
                        SpawnPowerUp(brick);
                    }
                    else
                    {
                        ShakeTime += 0.2f;
                        Effects.Shake = true;
                    }

                    if (!(Ball.PassThrough && !Ball.IsSolid))
                    {
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
        }

        auto result = CheckCollision(Ball, Player);
        if (!Ball.Stuck && result.Collision)
        {
            float centerBoard = Player.Position.x + Player.Size.x / 2.0f;
            float distance = (Ball.Position.x + Ball.Radius) - centerBoard;
            float proportion = distance / (Player.Size.x / 2.0f);

            float strength = 2.0f;

            auto speed = glm::length(Ball.Velocity);

            Ball.Stuck = Ball.Sticky;
            Ball.Velocity =
                glm::normalize(
                    glm::vec2(
                        BallObject::InitialVelocity.x * proportion * strength,
                        -1.0f * abs(Ball.Velocity.y)))
                * speed;
        }

        for (auto& powerUp : PowerUps)
        {
            if (!powerUp.Destroyed)
            {
                if (powerUp.Position.y >= Height)
                    powerUp.Destroyed = true;

                auto powerUpCollision = CheckCollision(Player, powerUp);
                if (powerUpCollision)
                {
                    ActivatePowerUp(powerUp);
                    powerUp.Destroyed = true;
                    powerUp.Activated = true;
                }
            }
        }
    }

    bool ShouldSpawn(unsigned int chance)
    {
        return rand() % chance == 0;
    }

    void Game::SpawnPowerUp(GameObject& block)
    {
        if (ShouldSpawn(75))
            PowerUps.push_back(
                PowerUp(
                    PowerUpType::Speed,
                    glm::vec3(0.5f, 0.5f, 1.0f),
                    0.0f,
                    block.Position,
                    ResourceManager::GetTexture("speed")));

        if (ShouldSpawn(75))
            PowerUps.push_back(
                PowerUp(
                    PowerUpType::Sticky,
                    glm::vec3(1.5f, 0.5f, 1.0f),
                    20.0f,
                    block.Position,
                    ResourceManager::GetTexture("sticky")));

        if (ShouldSpawn(75))
            PowerUps.push_back(
                PowerUp(
                    PowerUpType::PassThrough,
                    glm::vec3(0.5f, 1.0f, 0.5f),
                    10.0f,
                    block.Position,
                    ResourceManager::GetTexture("passthrough")));

        if (ShouldSpawn(75))
            PowerUps.push_back(
                PowerUp(
                    PowerUpType::PadSizeIncrease,
                    glm::vec3(1.0f, 0.5f, 0.4f),
                    0.0f,
                    block.Position,
                    ResourceManager::GetTexture("increase")));

        if (ShouldSpawn(15))
            PowerUps.push_back(
                PowerUp(
                    PowerUpType::Confuse,
                    glm::vec3(1.0f, 0.3f, 0.3f),
                    15.0f,
                    block.Position,
                    ResourceManager::GetTexture("confuse")));

        if (ShouldSpawn(15))
            PowerUps.push_back(
                PowerUp(
                    PowerUpType::Chaos,
                    glm::vec3(0.9f, 0.3f, 0.3f),
                    15.0f,
                    block.Position,
                    ResourceManager::GetTexture("chaos")));
    }

    bool Game::IsOtherPowerUpActive(PowerUpType type)
    {
        for (auto const& powerUp : PowerUps)
            if (powerUp.Activated && powerUp.Type == type)
                return true;

        return false;
    }

    void Game::ActivatePowerUp(PowerUp& powerUp)
    {
        switch (powerUp.Type)
        {
        case PowerUpType::Chaos:
        {
            if (!Effects.Confuse)
                Effects.Chaos = true;
        } break;
        case PowerUpType::Confuse:
        {
            if (!Effects.Chaos)
                Effects.Confuse = true;
        } break;
        case PowerUpType::PadSizeIncrease:
        {
            Player.Size.x += 50;
        } break;
        case PowerUpType::PassThrough:
        {
            Ball.PassThrough = true;
            Ball.Color = glm::vec3(1.0f, 0.5f, 0.5f);
        } break;
        case PowerUpType::Speed:
        {
            Ball.Velocity *= 1.2f;
        } break;
        case PowerUpType::Sticky:
        {
            Ball.Sticky = true;
            Player.Color = glm::vec3(1.0f, 0.5f, 1.0f);
        } break;
        }
    }

    void Game::Render()
    {
        if (State == GameState::Active || State == GameState::Pause)
        {
            Effects.BeginRender();

            Renderer.shader.Use();

            auto background = ResourceManager::GetTexture("background");
            Renderer.DrawSprite(
                background,
                glm::vec2(0.0f, 0.0f),
                glm::vec2((float)Width, (float)Height));

            Levels[Level].Draw(Renderer);
            Player.Draw(Renderer);
            Ball.Draw(Renderer);
            for (auto& powerUp : PowerUps)
                if (!powerUp.Destroyed)
                    powerUp.Draw(Renderer);

            Particles.Draw();

            Effects.EndRender();
            Effects.Render((float)glfwGetTime());
        }
    }

    void Game::ResetLevel()
    {
        Levels[0].Load("assets/levels/one.lvl", Width, Height / 2);

        PowerUps.clear();
    }

    void Game::ResetPlayer()
    {
        Player.Size = PLAYER_SIZE;
        Player.Position = glm::vec2(Width / 2.0f - PLAYER_SIZE.x / 2.0f, Height - PLAYER_SIZE.y);
        Player.Color = glm::vec3(1.0f);

        Ball.Reset(
            Player.Position + glm::vec2(
                PLAYER_SIZE.x / 2.0f - BallObject::InitialRadius,
                -(BallObject::InitialRadius * 2.0f)),
            BallObject::InitialVelocity);

        Effects.Chaos = false;
        Effects.Confuse = false;
    }

}