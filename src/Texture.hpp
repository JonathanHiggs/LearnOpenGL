#pragma once

#include <iostream>
#include <string>

#include <Glad/glad.h>

#include <stb/stb_image.h>


class Texture
{
public:
    unsigned int Id;

    Texture(std::string const& path)
    {
        stbi_set_flip_vertically_on_load(1);

        stbi_uc* data = nullptr;
        int width, height, channels;

        data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        GLenum internalFormat = 0, dataFormat = 0;

        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        else
        {
            std::cout << "Bad image format " << path << std::endl;
        }

        glGenTextures(1, &Id);
        glBindTexture(GL_TEXTURE_2D, Id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "No image data " << path << std::endl;
        }

        stbi_image_free(data);
    }

    ~Texture()
    {
        glDeleteTextures(1, &Id);
    }

    void Use(unsigned int slot = 0) const
    {
        glBindTextureUnit(slot, Id);
    }
};