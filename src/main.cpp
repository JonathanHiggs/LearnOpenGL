#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

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

    //auto phongShader = Shader("assets/phong-shader.vert", "assets/phong-shader.frag");
    auto materialShader = Shader("assets/material-shader.vert", "assets/material-shader.frag");
    auto lightShader = Shader("assets/light-shader.vert", "assets/light-shader.frag");

    //auto texture = Texture("assets/wall.jpg");
    auto diffuseTexture = Texture("assets/container2.png");
    auto specularTexture = Texture("assets/container2_specular.png");

    float cubeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int vbo, cubeVao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    unsigned int lightVao;

    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);

    //glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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

        /*auto lightColor = glm::vec3(
            sin(time * 2.0f),
            sin(time * 0.7f),
            sin(time * 1.3f));*/

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        materialShader.Use();

        diffuseTexture.Use(0);
        specularTexture.Use(1);
        materialShader.SetInt("uMaterial.diffuse", 0);
        materialShader.SetInt("uMaterial.specular", 1);
        materialShader.SetFloat("uMaterial.shininess", 32.0f);
        //materialShader.SetVec3("uLight.position", lightPosition);
        //materialShader.SetVec3("uLight.direction", -0.2f, -1.0f, -0.3f);
        materialShader.SetVec3("uLight.position", camera.Position());
        materialShader.SetVec3("uLight.direction", camera.Front());
        materialShader.SetFloat("uLight.cutOff", glm::cos(glm::radians(12.5f)));
        materialShader.SetFloat("uLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        materialShader.SetVec3("uLight.ambient", lightColor * 0.25f);
        materialShader.SetVec3("uLight.diffuse", lightColor);
        materialShader.SetVec3("uLight.specular", glm::vec3(1.0f) * glm::length2(lightColor));
        materialShader.SetFloat("uLight.constant", 1.0f);
        materialShader.SetFloat("uLight.linear", 0.09f);
        materialShader.SetFloat("uLight.quadratic", 0.032f);

        materialShader.SetVec3("uViewPosition", camera.Position());

        materialShader.SetMat4("uView", camera.View());
        materialShader.SetMat4("uProjection", window.Projection());

        for (auto i = 0; i < 10; i++)
        {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            auto normal = glm::mat3(glm::transpose(glm::inverse(model)));

            materialShader.SetMat4("uModel", model);
            materialShader.SetMat3("uNormal", normal);

            glBindVertexArray(cubeVao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        lightShader.Use();
        lightShader.SetVec3("uLightColor", lightColor);
        lightShader.SetMat4("uView", camera.View());
        lightShader.SetMat4("uProjection", window.Projection());
        auto model = glm::mat4(1.0f);
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