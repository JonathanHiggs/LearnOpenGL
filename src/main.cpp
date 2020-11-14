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

//#include <direct.h>
//
//std::string GetCurrentDir() {
//    char buff[FILENAME_MAX];
//    _getcwd(buff, FILENAME_MAX);
//    auto current_woring_dir = std::string(buff);
//    return current_woring_dir;
//}


int main()
{
    //std::cout << GetCurrentDir() << std::endl;

    auto props = WindowProps{ "LearnOpenGL" , 1600, 1200 };

    auto camera = Camera(
        glm::vec3(1.15f, 1.2f, 2.8f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -105.0f,
        -18.0f);

    auto window = Window(props, &camera);

    auto shader = Shader("assets/shader.vert", "assets/shader.frag");
    auto lightShader = Shader("assets/light-shader.vert", "assets/light-shader.frag");

    //auto texture = Texture("assets/wall.jpg");

    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int vbo, cubeVao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int lightVao;

    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);

    //glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    auto time = (float)glfwGetTime();

    auto lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
    auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    while (!window.ShouldClose())
    {
        lastFrame = time;
        time = (float)glfwGetTime();
        deltaTime = time - lastFrame;

        window.Poll();
        window.Update(deltaTime);
        camera.Update(deltaTime);

        glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        auto normal = glm::mat3(glm::transpose(glm::inverse(model)));

        shader.Use();
        shader.SetVec3("uObjectColor", 1.0f, 0.5f, 0.31f);
        shader.SetVec3("uLightColor", lightColor);
        shader.SetVec3("uLightPosition", lightPosition);
        shader.SetMat4("uModel", model);
        shader.SetMat3("uNormal", normal);
        shader.SetVec3("uViewPosition", camera.Position());
        shader.SetMat4("uView", camera.View());
        shader.SetMat4("uProjection", window.Projection());

        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        lightShader.Use();
        lightShader.SetVec3("uLightColor", lightColor);
        lightShader.SetMat4("uView", camera.View());
        lightShader.SetMat4("uProjection", window.Projection());
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.SetMat4("uModel", model);

        glBindVertexArray(lightVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.SwapBuffers();
    }

    glDeleteVertexArrays(1, &cubeVao);
    glDeleteVertexArrays(1, &lightVao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}