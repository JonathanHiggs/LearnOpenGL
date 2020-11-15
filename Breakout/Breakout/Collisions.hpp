#pragma once

#include <tuple>

#include <Breakout/BallObject.hpp>
#include <Breakout/GameObject.hpp>


namespace Breakout
{

    enum class Direction
    {
        Up,
        Right,
        Down,
        Left
    };

    struct Collision
    {
        bool Collision;
        Direction Direction;
        glm::vec2 Difference;
    };

    Direction VectorDirection(glm::vec2 target);

    bool CheckCollision(GameObject const& one, GameObject const& two);
    Collision CheckCollision(BallObject const& one, GameObject const& two);

}