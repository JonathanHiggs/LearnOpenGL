#pragma once

#include <map>

#include <Glad/glad.h>
#include <glm/glm.hpp>

#include <Breakout/Shader.hpp>
#include <Breakout/Texture.hpp>


namespace Breakout
{

    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // size of glyph
        glm::ivec2   Bearing;   // offset from baseline to left/top of glyph
        unsigned int Advance;   // horizontal offset to advance to next glyph
    };


    // A renderer class for rendering text displayed by a font loaded using the
    // FreeType library. A single font is loaded, processed into a list of Character
    // items for later rendering.
    class TextRenderer
    {
    public:
        std::map<char, Character> Characters;
        Shader TextShader;

        TextRenderer(unsigned int width, unsigned int height);

        void Load(std::string font, unsigned int fontSize);

        void RenderText(std::string text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));

        glm::vec2 Measure(std::string text, float scale = 1.0f);

    private:
        // render state
        unsigned int VAO, VBO;
    };

}