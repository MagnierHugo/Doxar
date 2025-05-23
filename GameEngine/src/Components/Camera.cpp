#include "Engine.hpp"

#include "Modules/RenderModule.hpp"
#include "Modules/InputManager.hpp"
#include "Modules/Time.hpp"

#include "Components/Camera.hpp"
#include "Components/Transform.hpp"

#include "Core/Maths/trigonometric.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "Core/Maths/gtx/euler_angles.hpp"


Camera::ComponentRegistrer<Camera> Camera::componentRegistrer;

void Camera::Awake() {
	RenderModule::SubscribeCamera(this);
}

void Camera::MakeMainCamera() {
	RenderModule::MakeMainCamera(this);
}

void Camera::OnDestroy() {
	RenderModule::UnSubscribeCamera(this);
}

void Camera::Update() {
	Move();

	if (InputManager::IsCursorEnabled() && !InputManager::GetMouseButton(GLFW_MOUSE_BUTTON_2)) { return; }

	glm::vec2 mouseDelta =
		InputManager::GetMouseDelta() *
		(InputManager::GetKey(GLFW_KEY_J) ? mouseSensitivityBoost : 1.f) *
		mouseSensitivity;
		;
	yaw += mouseDelta.x;
	pitch -= mouseDelta.y;

	UpdateVectors();
}

glm::mat4x4 Camera::GetViewMatrix() const {
	return glm::lookAt(Transform()->position, front + Transform()->position, up);
}

void Camera::Move() {
	int value;

	if ((value = InputManager::GetKey(GLFW_KEY_W) - InputManager::GetKey(GLFW_KEY_S)) != 0) {

		Transform()->position += front * (movementSpeed * Time::deltaTime * value);
	}
	
	if ((value = InputManager::GetKey(GLFW_KEY_D) - InputManager::GetKey(GLFW_KEY_A)) != 0) {

		Transform()->position += right * (movementSpeed * Time::deltaTime * value);
	}

	if ((value = InputManager::GetKey(GLFW_KEY_SPACE) - InputManager::GetKey(GLFW_KEY_LEFT_SHIFT)) != 0) {
		Transform()->position += up * (movementSpeed * Time::deltaTime * value);
	}
}
