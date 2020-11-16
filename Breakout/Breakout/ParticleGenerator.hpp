#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Breakout/GameObject.hpp>
#include <Breakout/Particle.hpp>
#include <Breakout/Shader.hpp>
#include <Breakout/Texture.hpp>


namespace Breakout
{

    class ParticleGenerator
    {
    public:
        ParticleGenerator(Shader shader, Texture2D texture, size_t amount);

        ParticleGenerator(ParticleGenerator&& other) noexcept;

        ~ParticleGenerator();

        void Update(
            float deltaTime,
            GameObject const& object,
            size_t newParticles,
            glm::vec2 offset = glm::vec2(0.0f));

        void Draw();

    private:
        void Init();

        size_t FirstUnusedParticle();

        void RespawnParticle(
            Particle& particle,
            GameObject const& object,
            glm::vec2 offset = glm::vec2(0.0f));

    private:
        std::vector<Particle> particles;
        size_t amount;
        size_t lastUsedParticle = 0u;

        Shader shader;
        Texture2D texture;
        unsigned int vao;
    };

}