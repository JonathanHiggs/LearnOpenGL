#include <Breakout/PostProcessor.hpp>

#include <iostream>

#include <glad/glad.h>


namespace Breakout
{

    PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height)
        : Confuse(false)
        , Chaos(false)
        , Shake(false)
        , width(width)
        , height(height)
        , postProcessingShader(shader)
        , texture()
    {
        InitFramebuffers();
        InitRenderData();

        postProcessingShader.SetInteger("scene", 0, true);

        float offset = 1.0f / 300.0f;
        float offsets[9][2] = {
            { -offset,  offset  },
            {  0.0f,    offset  },
            {  offset,  offset  },
            { -offset,  0.0f    },
            {  0.0f,    0.0f    },
            {  offset,  0.0f    },
            { -offset, -offset  },
            {  0.0f,   -offset  },
            {  offset, -offset  }
        };

        glUniform2fv(glGetUniformLocation(postProcessingShader.ID, "offsets"), 9, (float*)offsets);

        int edge_kernel[9] = {
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1
        };

        glUniform1iv(glGetUniformLocation(postProcessingShader.ID, "edge_kernel"), 9, edge_kernel);

        float blur_kernel[9] = {
            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
            2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
        };

        glUniform1fv(glGetUniformLocation(postProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
    }

    void PostProcessor::BeginRender()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, msfbo);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void PostProcessor::EndRender()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, msfbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void PostProcessor::Render(float time)
    {
        postProcessingShader.Use();
        postProcessingShader.SetFloat("time", time);
        postProcessingShader.SetInteger("confuse", Confuse);
        postProcessingShader.SetInteger("chaos", Chaos);
        postProcessingShader.SetInteger("shake", Shake);

        glActiveTexture(GL_TEXTURE0);
        texture.Bind();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
    }

    void PostProcessor::InitFramebuffers()
    {
        glGenFramebuffers(1, &msfbo);
        glGenFramebuffers(1, &fbo);
        glGenRenderbuffers(1, &rbo);

        glBindFramebuffer(GL_FRAMEBUFFER, msfbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO\n";

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        texture.Generate(width, height, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.ID, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO\n";

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void PostProcessor::InitRenderData()
    {
        unsigned int vbo;
        float vertices[] = {
            // pos          // tex
            -1.0f, -1.0f,   0.0f, 0.0f,
             1.0f,  1.0f,   1.0f, 1.0f,
            -1.0f,  1.0f,   0.0f, 1.0f,

            -1.0f, -1.0f,   0.0f, 0.0f,
             1.0f, -1.0f,   1.0f, 0.0f,
             1.0f,  1.0f,   1.0f, 1.0f
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

}