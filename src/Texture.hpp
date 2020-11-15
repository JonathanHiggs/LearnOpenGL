#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <Glad/glad.h>

#include <stb/stb_image.h>


enum class TextureType
{
    Diffuse,
    Specular,
    Normal,
    Height,
    Unknown
};


std::string const & ToString(TextureType type)
{
    switch (type)
    {
    case TextureType::Diffuse:
    {
        static std::string name = "texture_diffuse";
        return name;
    } break;

    case TextureType::Specular:
    {
        static std::string name = "texture_specular";
        return name;
    } break;

    case TextureType::Normal:
    {
        static std::string name = "texture_normal";
        return name;
    } break;

    case TextureType::Height:
    {
        static std::string name = "texture_height";
        return name;
    } break;

    default:
    {
        std::cout << "ERROR::TEXTURE_TYPE Unknown texture type";
        static std::string name = "unknown";
        return name;
    } break;
    }
}


class Texture
{
public:
    unsigned int Id;
    TextureType const Type;

    Texture(std::string const& path, TextureType type)
        : Type(type)
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

    Texture(Texture const& other) = delete;

    Texture(Texture&& other)
        : Id(other.Id)
        , Type(other.Type)
    {
        other.Id = 0u;
    }

    ~Texture()
    {
        if (Id != 0u)
            glDeleteTextures(1, &Id);
    }

    Texture operator = (Texture const& other) = delete;

    void Use(unsigned int slot = 0u) const
    {
        glBindTextureUnit(slot, Id);
    }
};


using TexturePtr = std::shared_ptr<Texture>;


class TextureManager
{
public:
    TextureManager()
        : cache()
    { }


    TexturePtr LoadTexture(std::string const& path, TextureType type)
    {
        auto it = cache.find(path);
        if (it != cache.end())
            return it->second;

        auto texture = std::make_shared<Texture>(path, type);
        cache[path] = texture;

        return texture;
    }


private:
    std::map<std::string, TexturePtr> cache;
};