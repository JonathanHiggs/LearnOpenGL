#include <Breakout/TextRenderer.hpp>

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Breakout/ResourceManager.hpp>


namespace Breakout
{

    TextRenderer::TextRenderer(unsigned int width, unsigned int height)
    {
        TextShader = ResourceManager::LoadShader("assets/shaders/text-shader.vert", "assets/shaders/text-shader.frag", nullptr, "text");
        TextShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
        TextShader.SetInteger("text", 0);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void TextRenderer::Load(std::string font, unsigned int fontSize)
    {
        Characters.clear();

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not initialize FreeType Library\n";

        FT_Face face;
        if (FT_New_Face(ft, font.c_str(), 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font: " << font << std::endl;

        FT_Set_Pixel_Sizes(face, 0, fontSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (GLubyte c = 0u; c < 128u; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph '" << (char)c << "'\n";
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Characters.insert(std::pair<char, Character>(
                c,
                Character{
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    (unsigned int)face->glyph->advance.x}));
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
    {
        TextShader.Use();
        TextShader.SetVector3f("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y + (Characters['H'].Bearing.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 0.0f },
                { xpos,     ypos,       0.0f, 0.0f },

                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glm::vec2 TextRenderer::Measure(std::string text, float scale)
    {
        auto width = 0.0f;
        auto height = (Characters['H'].Bearing.y) * scale;

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            width += (ch.Advance >> 6) * scale;
        }

        return glm::vec2(width, height);
    }

}