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

    Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

    int Run(int argc, char* argv[])
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, false);

        auto* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
        glfwMakeContextCurrent(window);

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

        Breakout.Init();

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwPollEvents();

            Breakout.ProcessInput(deltaTime);
            Breakout.Update(deltaTime);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            Breakout.Render();

            glfwSwapBuffers(window);
        }

        ResourceManager::Clear();

        glfwTerminate();
        return 0;
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (key >= 0 && key < 1024)
        {
            if (action == GLFW_PRESS)
                Breakout.Keys[key] = true;
            else if (action == GLFW_RELEASE)
                Breakout.Keys[key] = false;
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