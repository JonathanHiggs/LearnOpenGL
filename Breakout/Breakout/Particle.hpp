#pragma once

#include <glm/glm.hpp>


namespace Breakout
{

    class Particle
    {
    public:
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 Color;
        float Life;

        Particle();
    };

}