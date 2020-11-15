#pragma once

#include <map>
#include <string>

#include <Glad/glad.h>

#include <Breakout/Texture.hpp>
#include <Breakout/Shader.hpp>


namespace Breakout
{

    class ResourceManager
    {
    public:
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture2D> Textures;

    public:
        ResourceManager() = delete;

        static Shader LoadShader(
            const char* vShaderFile,
            const char* fShaderFile,
            const char* gShaderFile,
            std::string name);

        static Shader GetShader(std::string const& name);

        static Texture2D LoadTexture(const char* file, bool alpha, std::string name);

        static Texture2D GetTexture(std::string name);

        static void Clear();

    private:
        static Shader LoadShaderFromFile(
            const char* vShaderFile,
            const char* fShaderFile,
            const char* gShaderFile = nullptr);

        static Texture2D LoadTextureFromFile(const char* file, bool alpha);
    };

}