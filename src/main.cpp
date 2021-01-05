#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <chrono>
#include <string>

#include "Renderer/ShaderProgram.h"
#include "Resource/RecourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"


GLfloat point[] = {
    0.0f,  50.f, 0.0f,
    50.f, -50.f, 0.0f,
   -50.f, -50.f, 0.0f
};

GLfloat colors[] = {
    0.0f, 0.0f, 1.0f,
    0.9f, 0.9f, 0.0f,
    0.9f, 0.9f, 0.0f
};

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

glm::ivec2 g_windowSize(640, 480);

bool isEagle = false;

void glfwWindowSizeCallback(GLFWwindow* ptrWindow, int weight, int height) {
    g_windowSize.x = weight;
    g_windowSize.y = height;
    glViewport(0,0, g_windowSize.x, g_windowSize.y); //показуємо, де хочемо малювати
}

void glfwKeyCallback(GLFWwindow* ptrWindow, int key, int scancode, int action, int mode) {
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(ptrWindow, GL_TRUE);
    }
    if ((key == GLFW_KEY_ENTER) && (action == GLFW_PRESS)) {
       isEagle = !isEagle;
    }
}
int main(int argc, char** argv)
{
    
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //для перевірки, що у юзера версія 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* ptrwindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle City", nullptr, nullptr); //створення вікна
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
	if(!gladLoadGL()) {
		std::cout<<"Can't load GLAD!"<<std::endl;
		return -1;
	}
    
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	/*std::cout<<"OpenGL "<< GLVersion.major << "."<< GLVersion.minor<<std::endl;*/
	glClearColor(0, 1, 0, 1);
    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader program:" << "Dedault Shader" << std::endl;
            return -1;

        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create shader program:" << "SpriteShader" << std::endl;
            return -1;

        }
        
        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        std::vector<std::string> subTexturesNames = { 
            "eagle",
            "deadEagle", 
            "respawn1", 
            "respawn2", 
            "respawn3", 
            "respawn4", 
            "topLeftBlock",
            "bottomRightBlock", 
            "bottomRightBlock", 
            "beton"};
        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTexturesNames), 16, 16);

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "block");
        pSprite->setPosition(glm::vec2(300, 100));

        auto pAnimatedSprite = resourceManager.loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "eagle");
        pAnimatedSprite->setPosition(glm::vec2(300, 300));

        std::vector < std::pair < std::string, uint64_t>> eagleState;

        eagleState.emplace_back(std::make_pair <std::string, uint64_t>("eagle", 1e9));
        eagleState.emplace_back(std::make_pair <std::string, uint64_t>("deadEagle", 1e9));

       
        std::vector < std::pair < std::string, uint64_t>> respawnState;


        respawnState.emplace_back(std::make_pair <std::string, uint64_t>("respawn1", 1000));
        respawnState.emplace_back(std::make_pair <std::string, uint64_t>("respawn2", 1000));
        respawnState.emplace_back(std::make_pair <std::string, uint64_t>("respawn3", 1000));
        respawnState.emplace_back(std::make_pair <std::string, uint64_t>("respawn4", 1000));

        pAnimatedSprite->insertState("eagleState", std::move(eagleState));
        pAnimatedSprite->insertState("respawnState", std::move(respawnState));

        pAnimatedSprite->setState(std::basic_string("respawnState"));

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 200.f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 200.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the  closes the window */
        while (!glfwWindowShouldClose(ptrwindow))
        {
            if (isEagle)
            {
                pAnimatedSprite->setState(std::basic_string("eagleState"));
            }
            else
            {
                pAnimatedSprite->setState(std::basic_string("respawnState"));
            }
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            pAnimatedSprite->update(duration);
       
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->render();
            pAnimatedSprite->render();
            /* Swap front and back buffers */
            glfwSwapBuffers(ptrwindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}