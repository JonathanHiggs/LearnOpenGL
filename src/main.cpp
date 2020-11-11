#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Texture.hpp"


unsigned int ScreenWidth = 1600;
unsigned int ScreenHeight = 1200;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
const float sensitivity = 0.1f;

float lastX = 400;
float lastY = 300;

float yaw = 0;
float pitch = 0;
float fov = 45.0f;


void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;

    glViewport(0, 0, width, height);
}

void OnCursorPosChanged(GLFWwindow* window, double x, double y)
{
    if (firstMouse)
    {
        lastX = (float)x;
        lastY = (float)y;
        firstMouse = false;
    }

    float xOffset = ((float)x - lastX) * sensitivity;
    float yOffset = (lastY - (float)y) * sensitivity;

    lastX = (float)x;
    lastY = (float)y;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    auto direction = glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

    cameraFront = glm::normalize(direction);
}

void OnMouseScrolled(GLFWwindow* window, double xOffset, double yOffset)
{
    fov -= (float)yOffset;

    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 60.0f)
        fov = 60.0f;
}


void ProcessInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


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

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", NULL, NULL);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChanged);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, OnCursorPosChanged);
    glfwSetScrollCallback(window, OnMouseScrolled);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, ScreenWidth, ScreenHeight);
    glEnable(GL_DEPTH_TEST);

    auto shader = Shader("../../../../src/assets/shader.vert", "../../../../src/assets/shader.frag");
    auto texture = Texture("D:\\dev\\graphics\\LearnOpenGL\\src\\assets\\wall.jpg");


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

    while (!glfwWindowShouldClose(window))
    {
        lastFrame = time;
        time = (float)glfwGetTime();
        deltaTime = time - lastFrame;

        ProcessInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        auto projection = glm::perspective(glm::radians(fov), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);

        shader.Use();
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}