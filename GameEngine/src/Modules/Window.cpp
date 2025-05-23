#include <iostream>
#include <stdexcept>

#include "Modules/Window.hpp"
#include "Utilities/Logger.hpp"


//Window::Data Window::WindowData;

Window::Window(int width, int height, const char* title) : width(width), height(height), title(title), window(nullptr), shouldClose(false) {
    LOG_DEBUG("Window created");
    // WindowData.SetSize(width, height);
}

Window::~Window() { }

void Window::Init() {
    LOG_DEBUG("Window initializing");

    if (!glfwInit()) {
        LOG_CRITICAL("Window failed to initialize GLFW");
        throw std::runtime_error("Window failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Stop GLFW from Creating an OpenGL Context
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
		glfwTerminate(); //cleanup
        LOG_CRITICAL("Window failed to create window");
        throw std::runtime_error("Window failed to create window");
    }

    SetVSync(true); //Vsync because why not

    LOG_DEBUG("Window initialized successfully");
}

void Window::Update() {
    shouldClose = glfwWindowShouldClose(window);
}

void Window::Shutdown() {
    LOG_DEBUG("Window shutting down");
    if (window != nullptr) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

glm::ivec2 Window::GetSize() const {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    return glm::ivec2(w, h);
}

void Window::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::SetTitle(const char* newTitle) {
    title = newTitle;
    if (window) {
        glfwSetWindowTitle(window, title);
    }
}

void Window::ToggleFullscreen() {
    if (window == nullptr) { return; }

    static int windowed_x = 0;
    static int windowed_y = 0;

    static int windowed_width = width;
    static int windowed_height = height;

    if (glfwGetWindowMonitor(window)) {
        glfwSetWindowMonitor(window, nullptr, windowed_x, windowed_y, windowed_width, windowed_height, GLFW_DONT_CARE);
    }
    else {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (monitor == nullptr) { return; }

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (mode == nullptr) { return; }

        glfwGetWindowPos(window, &windowed_x, &windowed_y);
        glfwGetWindowSize(window, &windowed_width, &windowed_height);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
}