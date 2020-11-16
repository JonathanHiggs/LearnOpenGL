#include <Breakout/ParticleGenerator.hpp>


namespace Breakout
{

    ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, size_t amount)
        : amount(amount)
        , shader(shader)
        , texture(texture)
    {
        particles.reserve(amount);

        for (auto i = 0u; i < amount; i++)
            particles.emplace_back(Particle());

        Init();
    }

    ParticleGenerator::ParticleGenerator(ParticleGenerator&& other) noexcept
        : particles(std::move(other.particles))
        , amount(other.amount)
        , lastUsedParticle(other.lastUsedParticle)
        , shader(other.shader)
        , texture(other.texture)
        , vao(other.vao)
    {
        other.vao = 0u;
    }

    ParticleGenerator::~ParticleGenerator()
    {
        if (vao != 0u)
            glDeleteVertexArrays(1, &vao);
    }

    void ParticleGenerator::Update(
        float deltaTime,
        GameObject const & object,
        size_t newParticles,
        glm::vec2 offset)
    {
        for (auto i = 0u; i < newParticles; i++)
        {
            auto unusedParticle = FirstUnusedParticle();
            RespawnParticle(particles[unusedParticle], object, offset);
        }

        for (auto& particle : particles)
        {
            particle.Life -= deltaTime;
            if (particle.Life > 0.0f)
            {
                particle.Position -= particle.Velocity * deltaTime;
                particle.Color.a -= 2.5f * deltaTime;
            }
        }
    }

    void ParticleGenerator::Draw()
    {
        // Additive blending to give a glow
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        shader.Use();
        for (auto& particle : particles)
        {
            if (particle.Life > 0.0f)
            {
                shader.SetVector2f("offset", particle.Position);
                shader.SetVector4f("color", particle.Color);

                texture.Bind();

                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
            }
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void ParticleGenerator::Init()
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

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(vao);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    size_t ParticleGenerator::FirstUnusedParticle()
    {
        // Probably better performance to swap a dead particle so they stay at the end of the list
        // retrieval would be faster, and no branch statements in the update and render calls

        for (auto i = lastUsedParticle; i < amount; i++)
        {
            if (particles[i].Life <= 0.0f)
            {
                lastUsedParticle = i;
                return i;
            }
        }

        for (auto i = 0u; i < lastUsedParticle; i++)
        {
            if (particles[i].Life <= 0.0f)
            {
                lastUsedParticle = i;
                return i;
            }
        }

        lastUsedParticle = 0;
        return 0;
    }

    void ParticleGenerator::RespawnParticle(Particle& particle, GameObject const& object, glm::vec2 offset)
    {
        float random = ((rand() % 100) - 50) / 10.0f;
        float rColor = 0.5f + ((rand() % 100) / 100.0f);

        particle.Position = object.Position + random + offset;
        particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
        particle.Life = 1.0f;
        particle.Velocity = object.Velocity * 0.1f;
    }

}