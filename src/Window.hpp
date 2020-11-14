#pragma once

#include <iostream>
#include <string>

#include "Camera.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct WindowProps
{
    std::string const Title;
    unsigned int const Width;
    unsigned int const Height;
};


struct WindowData
{
    Camera * camera;
};


class Window
{
public:
    Window(WindowProps const& props, Camera * camera)
        : windowData{ camera }
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window\n";
            glfwTerminate();
            return;
        }

        camera->SetWindow(window);

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD\n";
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

        glViewport(0, 0, props.Width, props.Height);
        glEnable(GL_DEPTH_TEST);

        glfwSetWindowUserPointer(window, &windowData);
        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            auto* data = (WindowData*)glfwGetWindowUserPointer(window);
            glViewport(0, 0, width, height);
        });

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
            auto* data = (WindowData*)glfwGetWindowUserPointer(window);
            data->camera->OnCursorPosChanged(x, y);
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto* data = (WindowData*)glfwGetWindowUserPointer(window);
            data->camera->OnMouseScrolled(xOffset, yOffset);
        });
    }

    inline GLFWwindow* GlfwWindow() const noexcept { return window; }

    inline bool ShouldClose() const noexcept { return glfwWindowShouldClose(window); }

    inline glm::mat4 Projection() const noexcept
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
    }

    void Poll() const
    {
        glfwPollEvents();
    }

    void SwapBuffers() const
    {
        glfwSwapBuffers(window);
    }

    void Update(float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

private:
    GLFWwindow * window;
    WindowData windowData;

    float fov = 45.0f;
};