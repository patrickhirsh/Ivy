#include "Core/IvyPCH.h"
#include "Window/Window.h"

namespace Ivy
{
    Window::Window(GLFWwindow* window)
        : _window(window)
    {
        LOG_SYS("    New Window Context: " << HexString((int)(_window)));
        initRenderer();
        initEventDispatcher();
        _active = true;
    }

	Window::~Window()
	{
        delete _render;
        delete _event;
	}

    void Window::Update()
    {
        // set GLFW window context
        MakeCurrentContext();

        // rescale to window size
        GLint width, height;
        glfwGetWindowSize(_window, &width, &height);
        glViewport(0, 0, width, height);

        // clear previous frame
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render scene
        _render->ProcessRequests(_window);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    void Window::Tick(Ivy::Ref<StaticMesh> object)
    {
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

    void Window::SetSceneTranslation(float transX, float transY, float transZ)
    {
        _render->SetSceneTranslation(cy::Vec3f(transX, transY, transZ));
    }

    void Window::SetSceneRotation(float angleX, float angleY, float angleZ)
    {
        _render->SetSceneRotation(angleX, angleY, angleZ);
    }

    void Window::initRenderer()
    {
        MakeCurrentContext();
        _render = new Ivy::Render();
        LOG_SYS("    Initialized Render System");
    }

    void Window::initEventDispatcher()
    {
        MakeCurrentContext();
        _event = new Ivy::EventDispatcher();

        // TODO: handle this in a way that doesn't delete out from under the user...
        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            auto e = EventWindowClosed(ivyWindow);
            ivyWindow->_event->Fire(e);
            ivyWindow->_active = false; });

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
                auto e = EventMouseButtonReleased(button, mods);
                ivyWindow->_event->Fire(e); }
            else {
                auto e = EventMouseButtonPressed(button, mods);
                ivyWindow->_event->Fire(e); } });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            auto e = EventMouseMoved(xPos, yPos);
            ivyWindow->_event->Fire(e); });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            Window* ivyWindow = (Window*)glfwGetWindowUserPointer(window);
            auto e = EventMouseScrolled(xOffset, yOffset);
            ivyWindow->_event->Fire(e); });

        LOG_SYS("    Initialized Event System");
    }

    void Window::MakeCurrentContext()
    {
        glfwMakeContextCurrent(_window);
        glfwSetWindowUserPointer(_window, this);
    }
}