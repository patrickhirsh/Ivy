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
                /* Error.. */ 
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
            /* Error.. */
            glfwTerminate();  
            return nullptr; 
        }

        // init core systems
        window->_render = new _Ivy::Render();
        
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
        _render->ProcessRequests(_window);

        if (_window)
        {
            
            glfwMakeContextCurrent(_window);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(_window);
            glfwPollEvents();
        }  
    }

    void Window::Draw(Ivy::Ref<Object> object)
    {
        _render->DrawRequest(object);
    }
}