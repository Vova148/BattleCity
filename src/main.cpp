#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* ptrWindow, int weight, int height) {
    g_windowSizeX = weight;
    g_windowSizeY = height;
    glViewport(0,0, g_windowSizeX, g_windowSizeY); //показуємо, де хочемо малювати
}

void glfwKeyCallback(GLFWwindow* ptrWindow, int key, int scancode, int action, int mode) {
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(ptrWindow, GL_TRUE);
    }
}
int main(void)
{
    GLFWwindow* ptrwindow;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //для перевірки, що у юзера версія 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    ptrwindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr); //створення вікна
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
	glClearColor(0,1,0,1);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(ptrwindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(ptrwindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}