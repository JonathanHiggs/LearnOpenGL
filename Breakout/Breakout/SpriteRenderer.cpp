#include <Breakout/SpriteRenderer.hpp>

#include <Glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>


namespace Breakout
{

    SpriteRenderer::SpriteRenderer(Shader& shader)
        : shader(shader)
    {
        InitRenderData();
    }

    SpriteRenderer::SpriteRenderer(SpriteRenderer&& other) noexcept
        : shader(other.shader)
        , quadVao(other.quadVao)
    {
        other.quadVao = 0u;
    }

    SpriteRenderer::~SpriteRenderer()
    {
        if (quadVao != 0u)
            glDeleteVertexArrays(1, &this->quadVao);
    }

    void SpriteRenderer::DrawSprite(
        Texture2D & texture,
        glm::vec2 position,
        glm::vec2 size,
        float rotate,
        glm::vec3 color)
    {
        shader.Use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));

        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

        model = glm::scale(model, glm::vec3(size, 1.0f));

        shader.SetMatrix4("model", model);
        shader.SetVector3f("spriteColor", color);

        glActiveTexture(GL_TEXTURE0);
        texture.Bind();

        glBindVertexArray(quadVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void SpriteRenderer::InitRenderData()
    {
        unsigned int vbo;

        float vertices[] = {
            // position     // texture
            0.0f, 1.0f,     0.0f, 1.0f,
            1.0f, 0.0f,     1.0f, 0.0f,
            0.0f, 0.0f,     0.0f, 0.0f,

            0.0f, 1.0f,     0.0f, 1.0f,
            1.0f, 1.0f,     1.0f, 1.0f,
            1.0f, 0.0f,     1.0f, 0.0f,
        };

        glGenVertexArrays(1, &this->quadVao);
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->quadVao);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

}