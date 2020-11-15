#pragma once

#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

#include <Glad/glad.h>
#include <glm/glm.hpp>


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TexturePtr> textures;

    Mesh(
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<TexturePtr> textures
    )
        : vertices(vertices)
        , indices(indices)
        , textures(textures)
    {
        SetupMesh();
    }

    void Draw(Shader& shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            auto type = textures[i]->Type;
            std::string name = ToString(type);

            if (type == TextureType::Diffuse)
                number = std::to_string(diffuseNr++);
            else if (type == TextureType::Specular)
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if (type == TextureType::Normal)
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (type == TextureType::Height)
                number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.Id, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i]->Id);
        }

        // draw mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    void SetupMesh()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};