#pragma once

#include <Breakout/Shader.hpp>
#include <Breakout/Texture.hpp>


namespace Breakout
{

    class PostProcessor
    {
    public:
        bool Confuse;
        bool Chaos;
        bool Shake;

        PostProcessor(Shader shader, unsigned int width, unsigned int height);

        void BeginRender();
        void EndRender();
        void Render(float time);

    private:
        void InitFramebuffers();
        void InitRenderData();

    private:
        unsigned int width;
        unsigned int height;

        Shader postProcessingShader;
        Texture2D texture;

        unsigned int msfbo;
        unsigned int fbo;
        unsigned int rbo;
        unsigned int vao;
    };

}