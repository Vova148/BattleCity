﻿#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>

#include "Resource/RecourceManager.h"
#include "Game/Game.h"
#include "Renderer/Renderer.h"


glm::ivec2 g_windowSize(13*16, 14*16);
std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize);


void glfwWindowSizeCallback(GLFWwindow* ptrWindow, int width, int height) {
    g_windowSize.x = width;
    g_windowSize.y = height;

    const float level_aspect_ratio = 13.f / 14.f;
    unsigned int viewPortWidth = g_windowSize.x;
    unsigned int viewPortHeight = g_windowSize.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortBottomOffset = 0;

    if (static_cast<float>(g_windowSize.x) / g_windowSize.y > level_aspect_ratio)
    {
        viewPortWidth = static_cast<unsigned int>(g_windowSize.y * level_aspect_ratio);
        viewPortLeftOffset = (g_windowSize.x - viewPortWidth) / 2;
    }
    else
    {
        viewPortHeight = static_cast<unsigned int>(g_windowSize.x / level_aspect_ratio);
        viewPortBottomOffset = (g_windowSize.y - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);

}

void glfwKeyCallback(GLFWwindow* ptrWindow, int key, int scancode, int action, int mode) {
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(ptrWindow, GL_TRUE);
    }
    g_game->setKey(key, action);
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
        g_game->init();
        auto lastTime = std::chrono::high_resolution_clock::now();

        
        /* Loop until the  closes the window */
        while (!glfwWindowShouldClose(ptrwindow))
        {

            /* Poll for and process events */
            glfwPollEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            g_game->update(duration);

            /* Render here */
            RenderEngine::Renderer::clear();

            g_game->render();
            /* Swap front and back buffers */
            glfwSwapBuffers(ptrwindow);

        }
        g_game = nullptr;
        ResourceManager::unloadAllResource();
    }
    glfwTerminate();
    return 0;
}