#include "IvyPCH.h"
#include "Window.h"

namespace Ivy
{
    std::vector<Window*> Window::_activeWindows = std::vector<Window*>();

    Window::Window(const char* name, int width, int height)
    {
        initWindow(name, width, height);
        initStartup();
        initRenderer();
        initEventDispatcher();
    }

	Window::~Window()
	{
        // terminate core systems
        delete _render;
        delete _event;

        // terminate GLFW if there are no more active windows
        if (_activeWindows.size() < 1)
        {
            glfwTerminate();
        }
	}

    void Window::Update()
    {
        IVY_ACTIVE_WINDOW_ONLY

        // set GLFW window context
        makeCurrentContext();

        // rescale to window size
        GLint width, height;
        glfwGetWindowSize(_window, &width, &height);
        glViewport(0, 0, width, height);

        // clear previous frame
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _render->ProcessRequests(_window);
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    void Window::Draw(Ivy::Ref<Object> object)
    {
        IVY_ACTIVE_WINDOW_ONLY

        _render->DrawRequest(object);
    }

    void Window::RegisterEventCallback(Ivy::EventCategory category, Ivy::EventCallback callback)
    {
        _event->Register(category, callback);
    }

    void Window::UnregisterEventCallback(Ivy::EventCategory category, Ivy::EventCallback callback)
    {
        _event->Unregister(category, callback);
    }

    void Window::initWindow(
        const char*                         name,
        int                                 width,
        int                                 height)
    {
        // if this is the first window we've created, init GLFW
        if (_activeWindows.size() < 1)
        {
            if (!glfwInit())
            {
                LOG_ERROR("GLFW failed to initialize!");
                _active = false;
                return;
            }
        }

        // init window
        _width = width;
        _height = height;
        _window = glfwCreateWindow(width, height, name, NULL, NULL);
        glfwGetWindowPos(_window, &_xPos, &_yPos);

        // validate GLFW window
        if (!_window)
        {
            LOG_ERROR("GLFW falied to create a new window!");
            glfwTerminate();
            _active = false;
            return;
        }

        // init GLFW
        makeCurrentContext();
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // if this is the first window we've created, init GLEW
        if (_activeWindows.size() < 1)
        {
            if (glewInit() != GLEW_OK)
            {
                LOG_ERROR("GLEW failed to initialize!");
                glfwTerminate();
                _active = false;
                return;
            }
        }
        _activeWindows.push_back(this);
    }

    void Window::initStartup()
    {
        IVY_ACTIVE_WINDOW_ONLY

        // startup info
        LOG_SYS("=== Initializing New Ivy Engine Window Context ===");
        LOG_SYS("Window ID: " << std::to_string((int)(_window)).c_str());
        LOG_SYS("  <OpenGL>");
        LOG_SYS("    Hardware Vendor:    " << (glGetString(GL_VENDOR) == GL_NONE ? "N/A" : (const char*)glGetString(GL_VENDOR)));
        LOG_SYS("    Hardware Renderer:  " << (glGetString(GL_RENDERER) == GL_NONE ? "N/A" : (const char*)glGetString(GL_RENDERER)));
        LOG_SYS("    OpenGL Version:     " << (glGetString(GL_VERSION) == GL_NONE ? "N/A" : (const char*)glGetString(GL_VERSION)));
        LOG_SYS("  <Core>");
    }

    void Window::initRenderer()
    {
        IVY_ACTIVE_WINDOW_ONLY

        LOG_SYS("    Initializing Render System...");
        makeCurrentContext();
        _render = new _Ivy::Render();
    }

    void Window::initEventDispatcher()
    {
        IVY_ACTIVE_WINDOW_ONLY

        LOG_SYS("    Initializing Event System...");
        makeCurrentContext();
        _event = new _Ivy::EventDispatcher();

        // TODO: handle this in a way that doesn't delete out from under the user...
        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            auto e = EventWindowClosed(ivyWindow);
            ivyWindow->_event->Fire(e);
            ivyWindow->setInactive(); });

        glfwSetWindowFocusCallback(_window, [](GLFWwindow* window, int focused) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            if (focused) {
                auto e = EventWindowFocused(ivyWindow);
                ivyWindow->_event->Fire(e); }
            else {
                auto e = EventWindowLostFocus(ivyWindow);
                ivyWindow->_event->Fire(e); } });

        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            auto e = EventWindowResized(ivyWindow, width, height);
            ivyWindow->_width = width;
            ivyWindow->_height = height;
            ivyWindow->_event->Fire(e); });

        glfwSetWindowPosCallback(_window, [](GLFWwindow* window, int xPos, int yPos) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            auto e = EventWindowMoved(ivyWindow, xPos, yPos);
            ivyWindow->_xPos = xPos;
            ivyWindow->_yPos = yPos;
            ivyWindow->_event->Fire(e); });

        // TODO: Map keycodes to platform / Ivy keycodes & handle unknown keys
        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            if (action == GLFW_RELEASE) {
                auto e = EventKeyReleased(key, mods);
                ivyWindow->_event->Fire(e); }
            else {
                auto e = EventKeyPressed(key, mods, action == GLFW_REPEAT);
                ivyWindow->_event->Fire(e); } });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            if (action == GLFW_RELEASE) {
                auto e = EventMouseButtonPressed(button, mods);
                ivyWindow->_event->Fire(e); }
            else {
                auto e = EventMouseButtonReleased(button, mods);
                ivyWindow->_event->Fire(e); } });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            auto e = EventMouseMoved(xPos, yPos);
            ivyWindow->_event->Fire(e); });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            auto e = EventMouseScrolled(xOffset, yOffset);
            ivyWindow->_event->Fire(e); });
    }

    void Window::setInactive()
    {
        // remove window from active windows
        auto pos = std::find(_activeWindows.begin(), _activeWindows.end(), this);
        if (pos != _activeWindows.end())
        {
            _activeWindows.erase(pos);
        }
        _active = false;
    }

    void Window::makeCurrentContext()
    {
        glfwMakeContextCurrent(_window);
        glfwSetWindowUserPointer(_window, this);
    }
}