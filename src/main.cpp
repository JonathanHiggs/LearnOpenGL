#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Window.hpp"

#include <direct.h>


std::string GetCurrentDir() {
    char buff[FILENAME_MAX];
    _getcwd(buff, FILENAME_MAX);
    auto current_woring_dir = std::string(buff);
    return current_woring_dir;
}


int main()
{
    std::cout << GetCurrentDir() << std::endl;

    auto props = WindowProps{ "LearnOpenGL" , 1600, 1200 };

    auto camera = Camera(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        1600, 1200);

    auto window = Window(props, &camera);

    auto shader = Shader("assets/shader.vert", "assets/shader.frag");
    auto texture = Texture("assets/wall.jpg");


    float vertices[] = {
        // positions            // colours          // texture coord
        // front
         0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,     // top right
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // bottom right
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // bottom left
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,     // top left
        // back
         0.5f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,     // top right
         0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // bottom right
        -0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // bottom left
        -0.5f,  0.5f, 0.5f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f      // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7
    };

    unsigned int vbo, vao, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    auto time = (float)glfwGetTime();

    while (!window.ShouldClose())
    {
        lastFrame = time;
        time = (float)glfwGetTime();
        deltaTime = time - lastFrame;

        window.Poll();
        window.Update(deltaTime);
        camera.Update(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        shader.SetMat4("view", camera.View());
        shader.SetMat4("projection", camera.Projection());

        texture.Use();

        glBindVertexArray(vao);
        for (auto i = 0; i < 6; i++)
        {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, time * glm::radians(5.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            shader.SetMat4("model", model);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        window.SwapBuffers();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}