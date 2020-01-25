#include "IvyPCH.h"
#include "Window.h"

namespace Ivy
{
    std::vector<Ivy::Ref<Window>>* Window::_activeWindows = new std::vector<Ivy::Ref<Window>>();

    Ivy::Ref<Window> Window::Create(
        const char*                         name,
        int									width,
        int									height)
    {
        // if we don't currently have any windows, init GLFW
        if (_activeWindows->size() < 1)
        {
            if (!glfwInit()) 
            { 
                LOG_ERROR("GLFW failed to initialize!");
                return nullptr; 
            }
        }

        // create new window
        Ivy::Ref<Window> window = Ivy::Ref<Window>(new Window()); 
        window->_width = width;
        window->_height = height;

        // create GLFW window
        window->_window = glfwCreateWindow(width, height, name, NULL, NULL);
        if (!window->_window) 
        { 
            LOG_ERROR("GLFW falied to create a new window!");
            glfwTerminate();
            return nullptr; 
        }
        glfwMakeContextCurrent(window->_window);

        // if this is the first window we've created, init GLEW
        if (_activeWindows->size() < 1)
        {
            if (glewInit() != GLEW_OK)
            {
                LOG_ERROR("GLEW failed to initialize!");
                glfwTerminate();
                return nullptr;
            }
        }

        // init core systems
        window->_render = new _Ivy::Render();

        // init OpenGL window
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective(60, (double)width / (double)height, 0.1, 100);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0, 0, -30);
        
        _activeWindows->push_back(window);
        return window;
    }

	Window::~Window()
	{
        // remove window from active windows
        auto pos = std::find(_activeWindows->begin(), _activeWindows->end(), Ivy::Ref<Window>(this));
        if (pos != _activeWindows->end())
        {
            _activeWindows->erase(pos);
        }

        // terminate GLFW if there are no more active windows
        if (_activeWindows->size() < 1)
        {
            glfwTerminate();
        }
	}

    void Window::Update()
    {
        glfwMakeContextCurrent(_window);

        // rescale to window size
        GLint width, height;
        glfwGetWindowSize(_window, &width, &height);
        glViewport(0, 0, width, height);

        // clear previous frame
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set perspective
        //glMatrixMode(GL_PROJECTION_MATRIX);
        //glLoadIdentity();
        //gluPerspective(60, (double)width / (double)height, 0.1, 100);

        //glMatrixMode(GL_MODELVIEW_MATRIX);
        //glTranslatef(0, 0, -5);

        _render->ProcessRequests(_window);

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    void Window::Draw(Ivy::Ref<Object> object)
    {
        _render->DrawRequest(object);
    }
}