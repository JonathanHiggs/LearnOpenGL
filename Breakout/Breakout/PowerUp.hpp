#pragma once

#include <glm/glm.hpp>

#include <Breakout/GameObject.hpp>
#include <Breakout/Texture.hpp>


namespace Breakout
{

    enum class PowerUpType
    {
        Speed,
        Sticky,
        PassThrough,
        PadSizeIncrease,
        Confuse,
        Chaos
    };

    class PowerUp : public GameObject
    {
    public:
        PowerUpType Type;
        float Duration;
        bool Activated;

        PowerUp(
            PowerUpType type,
            glm::vec3 color,
            float duration,
            glm::vec2 position,
            Texture2D texture);

        static glm::vec2 const InitialSize;
        static glm::vec2 const InitialVelocity;
    };

}