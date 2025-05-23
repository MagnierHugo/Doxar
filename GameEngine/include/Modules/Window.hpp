#pragma once

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Core/Maths/vec2.hpp"

#include "Module.hpp"



class Window : public Module {
private:
    GLFWwindow* window;
    int width;
    int height;
    const char* title;
    bool shouldClose;

public:
    Window(const int width = 1000, const int height = 600, const char* title = "Engine Window");
    ~Window();

#ifdef _WINDOWS
    operator HWND() {
        return glfwGetWin32Window(window);
    }
#else
#endif

    void Init() override;
    void Update() override;
    void Shutdown() override;

    GLFWwindow* GetWindow() const { return window; }
    HWND GetHWND() const { return glfwGetWin32Window(window);  }
    bool ShouldClose() const { return shouldClose; }
    void SetShouldClose(const bool value) { shouldClose = value; }

    glm::ivec2 GetSize() const;

    void SetVSync(const bool enabled);
    void SetTitle(const char* newTitle);
    void ToggleFullscreen();
};