#include <iostream>

#include "Engine.hpp"
#include "Modules/InputManager.hpp"
#include "Modules/Window.hpp"
#include "Utilities/Logger.hpp"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager() {

    if (instance != nullptr) {
        LOG_CRITICAL("InputManager instance already exists!");
        throw std::runtime_error("InputManager instance already exists!");
    }

    instance = this;
    LOG_DEBUG("InputManager created");
}

void InputManager::SetWindow(GLFWwindow* window) {
    this->window = window;

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void InputManager::Init() {
    LOG_DEBUG("InputManager initializing");

    Window* windowModule = Engine::GetInstance()->GetModule<Window>();
    if (!windowModule) {
        LOG_CRITICAL("InputManager window module not found!");
        throw std::runtime_error("InputManager window module not found!");
    }

    window = windowModule->GetWindow();

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void InputManager::ResetInputsState() {
    previousKeyStates = currentKeyStates;
    previousMouseButtonStates = currentMouseButtonStates;

    instance->lastMouseX = instance->mouseX;
    instance->lastMouseY = instance->mouseY;
    mouseMoved = false;

    scrollDelta = 0.0f;
}

void InputManager::Update() {

    ResetInputsState();

    glfwPollEvents();

    if (GetKeyDown(GLFW_KEY_LEFT_ALT)) {
        cursorEnabled = !cursorEnabled;
        glfwSetInputMode(window, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}

bool InputManager::GetWhetherAnyInput() {
    bool hasInput = GetAnyKeyDown() || GetAnyMouseButtonDown() || instance->mouseMoved || (instance->scrollDelta != 0.0f);
    if (hasInput) {
        LOG_DEBUG("InputManager input detected!");
    }
    return hasInput;
}

#pragma region Keyboard Inputs

bool InputManager::GetAnyKeyDown() {
    for (const auto& [key, pressed] : instance->currentKeyStates) {
        if (pressed && !instance->previousKeyStates[key]) {
            LOG_DEBUG("InputManager new key pressed: {}", key);
            return true;
        }
    }

    return false;
}

bool InputManager::GetWhichKeyDown(int& key) {
    for (const auto& [key_, pressed] : instance->currentKeyStates) {
        if (pressed && !instance->previousKeyStates[key_]) {
            LOG_DEBUG("InputManager key down detected: {}", key_);
            key = key_;
            return true;
        }
    }

    return false;
}

bool InputManager::GetKeyDown(const int key) {
    bool isDown = instance->currentKeyStates[key] && !instance->previousKeyStates[key];
    if (isDown) {
        LOG_DEBUG("InputManager specific key down: {}", key);
    }
    return isDown;
}

bool InputManager::GetKey(const int key) {
    bool isPressed = instance->currentKeyStates[key];
    if (isPressed) {
        LOG_DEBUG("InputManager key held: {}", key);
    }
    return isPressed;
}

bool InputManager::GetKeyUp(const int key) {
    bool isUp = !instance->currentKeyStates[key] && instance->previousKeyStates[key];
    if (isUp) {
        LOG_DEBUG("InputManager key released: {}", key);
    }
    return isUp;
}

#pragma endregion

#pragma region Mouse Inputs

#pragma region Buttons

bool InputManager::GetAnyMouseButtonDown() {
    for (const auto& [mouseButton, pressed] : instance->currentMouseButtonStates) {
        if (pressed && !instance->previousMouseButtonStates[mouseButton]) {
            LOG_DEBUG("InputManager new mouse button pressed: {}", mouseButton);
            return true; 
        }
    }

    return false;
}

bool InputManager::GetWhichMouseButtonDown(int& button) {
    for (const auto& [mouseButton, pressed] : instance->currentMouseButtonStates) {
        if (pressed && !instance->previousMouseButtonStates[mouseButton]) {
            button = mouseButton;
            LOG_DEBUG("InputManager mouse button down detected: {}", mouseButton);
            return true;
        }
    }

    return false;
}

bool InputManager::GetMouseButtonDown(const int button) {
    bool isDown = instance->currentMouseButtonStates[button] && !instance->previousMouseButtonStates[button];
    if (isDown) {
        LOG_DEBUG("InputManager specific mouse button down: {}", button);
    }
    return isDown;
}

bool InputManager::GetMouseButton(const int button) {
    bool isPressed = instance->currentMouseButtonStates[button];
    if (isPressed) {
        LOG_DEBUG("InputManager mouse button held: {}", button);
    }
    return isPressed;
}

bool InputManager::GetMouseButtonUp(const int button) {
    bool isUp = !instance->currentMouseButtonStates[button] && instance->previousMouseButtonStates[button];
    if (isUp) {
        LOG_DEBUG("InputManager mouse button released: {}", button);
    }
    return isUp;
}

#pragma endregion

#pragma region Position

glm::vec2 InputManager::GetMousePosition() {
    LOG_DEBUG("InputManager mouse position queried: {}, {}", instance->mouseX, instance->mouseY);
    return glm::vec2(static_cast<float>(instance->mouseX), static_cast<float>(instance->mouseY));
}

bool InputManager::GetWetherAnyMouseMotion() {
    if (instance->mouseMoved) {
        LOG_DEBUG("InputManager mouse motion detected");
    }
    return instance->mouseMoved;
}

glm::vec2 InputManager::GetMouseDelta() {

    glm::vec2 delta{
        static_cast<float>(instance->mouseX - instance->lastMouseX),
        static_cast<float>(instance->mouseY - instance->lastMouseY)
    };

    LOG_DEBUG("InputManager mouse delta: {}, {}", delta.x, delta.y);
    return delta;
}

#pragma endregion

#pragma region Wheel

float InputManager::GetMouseWheelDelta() {
    if (instance->scrollDelta != 0.0f) {
        LOG_DEBUG("InputManager mouse wheel delta: {}", instance->scrollDelta);
    }
    return instance->scrollDelta;
}

#pragma endregion

#pragma endregion

void InputManager::Shutdown() {
    LOG_DEBUG("InputManager shutting down");
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    instance->currentKeyStates[key] = (action != GLFW_RELEASE);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    instance->currentMouseButtonStates[button] = (action != GLFW_RELEASE);
}

void InputManager::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    instance->mouseX = xpos;
    instance->mouseY = ypos;
    instance->mouseMoved = true;
}

void InputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    instance->scrollDelta = (float)yoffset;
}

bool InputManager::IsCursorEnabled() {
    return instance->cursorEnabled;
}