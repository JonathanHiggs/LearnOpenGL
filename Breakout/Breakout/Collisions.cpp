#include <Breakout/Collisions.hpp>

#include <glm/gtx/norm.hpp>


namespace Breakout
{

    Direction VectorDirection(glm::vec2 target)
    {
        static glm::vec2 compass[] = {
            glm::vec2(0.0f,  1.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(0.0f, -1.0f),
            glm::vec2(0.0f, -1.0f),
        };

        auto max = 0.0f;
        int bestMatch = -1;

        for (auto i = 0u; i < 4u; i++)
        {
            auto dotProduct = glm::dot(glm::normalize(target), compass[i]);
            if (dotProduct > max)
            {
                max = dotProduct;
                bestMatch = i;
            }
        }

        return static_cast<Direction>(bestMatch);
    }

    bool CheckCollision(GameObject const& a, GameObject const& b)
    {
        bool collisionX = a.Position.x + a.Size.x >= b.Position.x
            && b.Position.x + b.Size.x >= a.Position.x;

        bool collisionY = a.Position.y + a.Size.y >= b.Position.y
            && b.Position.y + b.Size.y >= a.Position.y;

        return collisionX && collisionY;
    }

    Collision CheckCollision(BallObject const& one, GameObject const& two)
    {
        auto center = one.Position + one.Radius;
        auto aabbHalfExtends = glm::vec2(two.Size.x / 2.0f, two.Size.y / 2.0f);
        auto aabbCentre = glm::vec2(
            two.Position.x + aabbHalfExtends.x,
            two.Position.y + aabbHalfExtends.y);

        auto difference = center - aabbCentre;
        auto clamped = glm::clamp(difference, -aabbHalfExtends, aabbHalfExtends);
        auto closest = aabbCentre + clamped;
        difference = closest - center;

        if (glm::length(difference) < one.Radius)
            return Collision{ true, VectorDirection(difference), difference };
        else
            return Collision{ false, Direction::Up, glm::vec2(0.0f) };
    }

}