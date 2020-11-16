#pragma once

#include <glm/glm.hpp>

#include <Breakout/SpriteRenderer.hpp>
#include <Breakout/Texture.hpp>


namespace Breakout
{

    class GameObject
    {
    public:
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec2 Size;
        Texture2D Texture;
        glm::vec3 Color;
        bool IsSolid = false;
        bool Destroyed = false;

        GameObject(
            glm::vec2 const & position,
            glm::vec2 const & velocity,
            glm::vec2 const & size,
            Texture2D texture,
            glm::vec3 const& color = glm::vec3(1.0f),
            bool isSolid = false);

        void Draw(SpriteRenderer& renderer);
    };

}