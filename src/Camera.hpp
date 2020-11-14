#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
    Camera(
        glm::vec3 position,
        glm::vec3 front,
        glm::vec3 up,
        unsigned int screenHeight,
        unsigned int screenWidth
    )
        : window(nullptr)
        , position(position)
        , front(front)
        , up(up)
        , screenHeight(screenHeight)
        , screenWidth(screenWidth)
    { }


    inline glm::mat4 View() const noexcept
    {
        return glm::lookAt(position, position + front, up);
    }

    inline glm::mat4 Projection() const noexcept
    {
        return glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    }

    void Update(float deltaTime)
    {
        const float cameraSpeed = 2.5f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position += cameraSpeed * front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position -= cameraSpeed * front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
    }

    void OnCursorPosChanged(double x, double y)
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

        front = glm::normalize(direction);
    }

    void OnMouseScrolled(double xOffset, double yOffset)
    {
        fov -= (float)yOffset;

        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 60.0f)
            fov = 60.0f;
    }

    void OnFrameBufferSizeChanged(int width, int height)
    {
        screenWidth = width;
        screenHeight = height;
    }

    void SetWindow(GLFWwindow* glfwWindow)
    {
        window = glfwWindow;
    }

private:
    GLFWwindow* window;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    const float sensitivity = 0.1f;
    bool firstMouse = true;

    float lastX = 0;
    float lastY = 0;

    float pitch = 0;
    float yaw = 0;
    float fov = 45.0f;

    unsigned int screenHeight;
    unsigned int screenWidth;
};