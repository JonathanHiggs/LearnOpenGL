#include <Breakout/PowerUp.hpp>


namespace Breakout
{

    glm::vec2 const PowerUp::InitialSize = glm::vec2(90.0f, 30.0f);
    glm::vec2 const PowerUp::InitialVelocity = glm::vec2(0.0f, 200.0f);

    PowerUp::PowerUp(
        PowerUpType type,
        glm::vec3 color,
        float duration,
        glm::vec2 position,
        Texture2D texture
    )
        : GameObject(
            position,
            PowerUp::InitialVelocity,
            PowerUp::InitialSize,
            texture,
            color)
        , Type(type)
        , Duration(duration)
        , Activated(false)
    { }

}