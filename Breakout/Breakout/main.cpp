#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Breakout/Game.hpp>
#include <Breakout/ResourceManager.hpp>


namespace Breakout
{

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

    const unsigned int SCREEN_WIDTH = 1200;
    const unsigned int SCREEN_HEIGHT = 900;

    int Run(int argc, char* argv[])
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, false);

        auto* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD\n";
            return -1;
        }

        glfwSetKeyCallback(window, key_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto breakout = Game::Init(SCREEN_WIDTH, SCREEN_HEIGHT);
        glfwSetWindowUserPointer(window, &breakout);

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwPollEvents();

            breakout.ProcessInput(deltaTime);
            breakout.Update(deltaTime);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            breakout.Render();

            glfwSwapBuffers(window);
        }

        ResourceManager::Clear();

        glfwTerminate();
        return 0;
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        auto* breakout = (Game*)glfwGetWindowUserPointer(window);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (key >= 0 && key < 1024)
        {
            if (action == GLFW_PRESS)
                breakout->Keys[key] = true;
            else if (action == GLFW_RELEASE)
                breakout->Keys[key] = false;
        }
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

}


int main(int argc, char* argv[])
{
    return Breakout::Run(argc, argv);
}