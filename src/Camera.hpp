#pragma once

#include <iostream>

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
        glm::vec3 up
    )
        : window(nullptr)
        , position(position)
        , front(front)
        , up(up)
    {
        UpdateVectors();
    }


    Camera(
        glm::vec3 position,
        glm::vec3 up,
        float yaw,
        float pitch
    )
        : window(nullptr)
        , position(position)
        , front(glm::vec3(0.0f, 0.0f, -1.0f))
        , up(up)
        , yaw(yaw)
        , pitch(pitch)
    {
        UpdateVectors();
    }


    inline glm::mat4 View() const noexcept
    {
        return glm::lookAt(position, position + front, up);
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

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            position += cameraSpeed * up;

        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
        {
            std::cout << "position x: " << position.x << " y: " << position.y << " z: " << position.z << "\tpitch: " << pitch << "\tyaw: " << yaw << std::endl;
        }
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

        UpdateVectors();
    }

    void OnMouseScrolled(double xOffset, double yOffset)
    {
        fov -= (float)yOffset;

        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 60.0f)
            fov = 60.0f;
    }

    void SetWindow(GLFWwindow* glfwWindow)
    {
        window = glfwWindow;
    }

private:
    void UpdateVectors()
    {
        auto direction = glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

        front = glm::normalize(direction);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

private:
    GLFWwindow* window;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 const worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    const float sensitivity = 0.08f;
    bool firstMouse = true;

    float lastX = 0;
    float lastY = 0;

    float pitch = 0;
    float yaw = -90.0f;
    float fov = 45.0f;
};