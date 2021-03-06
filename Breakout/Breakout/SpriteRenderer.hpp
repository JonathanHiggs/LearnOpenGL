#pragma once

#include <glm/glm.hpp>

#include <Breakout/Shader.hpp>
#include <Breakout/Texture.hpp>


namespace Breakout
{

    class SpriteRenderer
    {
    public:
        Shader shader;

        SpriteRenderer(Shader& shader);
        SpriteRenderer(SpriteRenderer&& other) noexcept;

        ~SpriteRenderer();

        void DrawSprite(
            Texture2D& texture,
            glm::vec2 position,
            glm::vec2 size = glm::vec2(10.0f, 10.0f),
            float rotate = 0.0f,
            glm::vec3 color = glm::vec3(1.0f));

    private:
        void InitRenderData();

    private:
        unsigned int quadVao;
    };

}