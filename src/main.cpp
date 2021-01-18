#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>

#include "Resource/RecourceManager.h"
#include "Game/Game.h"
#include "Renderer/Renderer.h"


glm::ivec2 g_windowSize(640, 480);
Game g_game(g_windowSize);


void glfwWindowSizeCallback(GLFWwindow* ptrWindow, int weight, int height) {
    g_windowSize.x = weight;
    g_windowSize.y = height;
    RenderEngine::Renderer::setViewport(weight, height);
}

void glfwKeyCallback(GLFWwindow* ptrWindow, int key, int scancode, int action, int mode) {
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(ptrWindow, GL_TRUE);
    }
    g_game.setKey(key, action);
}
int main(int argc, char** argv)
{

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //äëÿ ïåðåâ³ðêè, ùî ó þçåðà âåðñ³ÿ 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* ptrwindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle City", nullptr, nullptr); //ñòâîðåííÿ â³êíà
    if (!ptrwindow)
    {
        std::cout << "glfwCreateWindow failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(ptrwindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(ptrwindow, glfwKeyCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(ptrwindow);
    if (!gladLoadGL()) {
        std::cout << "Can't load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << std::endl;
    std::cout << "OpenGL Version: " << RenderEngine::Renderer::getVersionStr() << std::endl;


    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
    {
        ResourceManager::setExecutablePath(argv[0]);
        g_game.init();
        auto lastTime = std::chrono::high_resolution_clock::now();

        
        /* Loop until the  closes the window */
        while (!glfwWindowShouldClose(ptrwindow))
        {

            /* Poll for and process events */
            glfwPollEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            g_game.update(duration);

            /* Render here */
            RenderEngine::Renderer::clear();

            g_game.render();
            /* Swap front and back buffers */
            glfwSwapBuffers(ptrwindow);

        }
        ResourceManager::unloadAllResource();
    }
    glfwTerminate();
    return 0;
}