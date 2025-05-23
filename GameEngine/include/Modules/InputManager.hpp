#pragma once

#include <unordered_map>

#include <GLFW/glfw3.h>

#include "Core/Maths/vec2.hpp"

#include "Module.hpp"



class InputManager : public Module {
private:

    enum class KeyReturn {
        Pressed = GLFW_PRESS,
        Released = GLFW_RELEASE,
        Repeat = GLFW_REPEAT
    };

    static InputManager* instance;
    struct GLFWwindow* window = nullptr;

    std::unordered_map<int, bool> currentKeyStates;
    std::unordered_map<int, bool> previousKeyStates;

    std::unordered_map<int, bool> currentMouseButtonStates;
    std::unordered_map<int, bool> previousMouseButtonStates;

    double mouseX = 0.0;
    double mouseY = 0.0;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    float scrollDelta = 0.0f;
    bool mouseMoved = false;
    bool cursorEnabled = true;

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
    InputManager();
    void SetWindow(GLFWwindow* window);
    void ResetInputsState();
    void Init() override;
    void Update() override;
    void Shutdown() override;

    static bool GetWhetherAnyInput();

    static bool GetAnyKeyDown();
    static bool GetWhichKeyDown(int& key);
    static bool GetKeyDown(const int key);
    static bool GetKey(const int key);
    static bool GetKeyUp(const int key);

    static bool GetAnyMouseButtonDown();
    static bool GetWhichMouseButtonDown(int& button);
    static bool GetMouseButtonDown(const int button);
    static bool GetMouseButton(const int button);
    static bool GetMouseButtonUp(const int button);

    static glm::vec2 GetMousePosition();
    static bool GetWetherAnyMouseMotion();
    static glm::vec2 GetMouseDelta();
    static float GetMouseWheelDelta();

    static bool IsCursorEnabled();
};