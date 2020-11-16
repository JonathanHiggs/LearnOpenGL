#include <Breakout/BallObject.hpp>


namespace Breakout
{

    glm::vec2 const BallObject::InitialVelocity = glm::vec2(150.0f, -500.0f);
    float const BallObject::InitialRadius = 17.5f;

    BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
        : GameObject(
            pos,
            velocity,
            glm::vec2(radius * 2.0f, radius * 2.0f),
            sprite,
            glm::vec3(1.0f))
        , Radius(radius)
        , Stuck(true)
        , Sticky(false)
        , PassThrough(false)
    { }


    glm::vec2 BallObject::Move(float deltaTime, unsigned int windowWidth)
    {
        auto width = (float)windowWidth - Size.x;

        if (!Stuck)
        {
            Position += Velocity * deltaTime;

            if (Position.x <= 0.0f)
            {
                Velocity.x = -Velocity.x;
                //Position.x = -Position.x;
                Position.x = 0.0f;
            }
            else if (Position.x >= width)
            {
                Velocity.x = -Velocity.x;
                //Position.x = 2.0f * width - Position.x;
                Position.x = width - Size.x;
            }

            if (Position.y <= 0.0f)
            {
                Velocity.y = -Velocity.y;
                //Position.y = -Position.y;
                Position.y = 0.0f;
            }
        }

        return Position;
    }

    void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
    {
        Position = position;
        Velocity = velocity;
        Stuck = true;
        Sticky = false;
        PassThrough = false;
    }

}