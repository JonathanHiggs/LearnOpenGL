#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <Glad/glad.h>

#include <glm/glm.hpp>


class Shader
{
public:
    unsigned int Id;

    Shader(std::string const& vertexPath, std::string const& fragmentPath)
    {
        auto vertexCode = ReadFile(vertexPath);
        auto fragmentCode = ReadFile(fragmentPath);

        const char* vertexShaderCode = vertexCode.c_str();
        const char* fragmentShaderCode = fragmentCode.c_str();

        int success;
        char infoLog[512];

        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);


        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        Id = glCreateProgram();

        glAttachShader(Id, vertexShader);
        glAttachShader(Id, fragmentShader);
        glLinkProgram(Id);

        glGetProgramiv(Id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(Id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader()
    {
        glDeleteProgram(Id);
        Id = 0;
    }

    void Use() { glUseProgram(Id); }

    void SetBool(std::string const& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
    }

    void SetInt(std::string const& name, int value) const
    {
        glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
    }

    void SetFloat(std::string const& name, float value) const
    {
        glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
    }

    void SetMat4(std::string const& name, glm::mat4 value) const
    {
        auto location = glGetUniformLocation(Id, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    std::string ReadFile(std::string const& path)
    {
        auto fileStream = std::ifstream(path.c_str(), std::ios::in | std::ios::binary);

        if (!fileStream.good())
            return "";

        fileStream.seekg(0, std::ios::end);
        auto size = fileStream.tellg();

        std::string result;
        result.resize(size);
        fileStream.seekg(0, std::ios::beg);
        fileStream.read(&result[0], size);
        fileStream.close();

        return result;
    }
};