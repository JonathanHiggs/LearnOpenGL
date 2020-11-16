#pragma once

#include <Breakout/GameObject.hpp>


namespace Breakout
{

    class BallObject : public GameObject
    {
    public:
        float Radius;
        bool Stuck;
        bool Sticky;
        bool PassThrough;

        BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

        glm::vec2 Move(float deltaTime, unsigned int windowWidth);
        void Reset(glm::vec2 position, glm::vec2 velocity);

        static glm::vec2 const  InitialVelocity;
        static float const InitialRadius;
    };

}