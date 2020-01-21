#include "Window.h"

namespace Ivy
{
    bool _exitFlag = false;

    void keyEventHandle(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            _exitFlag = true;
    }

	Window::Window()
	{
        GLFWwindow* window;

        // init
        if (!glfwInit()) { return; }

        // create window
        window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
        if (!window) { glfwTerminate(); return; }
        glfwMakeContextCurrent(window);

        // set event callbacks
        glfwSetKeyCallback(window, keyEventHandle);

        while (!glfwWindowShouldClose(window) && !_exitFlag)
        {
            glClearColor(1.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
	}

	Window::~Window()
	{

	}
}