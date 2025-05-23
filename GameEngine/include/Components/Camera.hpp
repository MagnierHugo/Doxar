	#pragma once

#include "Core/Maths/vec3.hpp"
#include "Core/Maths/vec4.hpp"
#include "Core/Maths/mat4x4.hpp"
#include "Core/Maths/ext/matrix_transform.hpp"

#include "Component.hpp"

class Camera : public Component
{
public:
	Camera() = default;

	void Awake() override;
	void Update() override;
	void MakeMainCamera();

	void OnDestroy() override;
	glm::mat4x4 GetViewMatrix() const;

	void OnInspect() override {
		ImGUI::BeginChild("CameraComponent");

		if (ImGUI::Button("Make main camera")) {
			MakeMainCamera();
		}
		ImGUI::EndChild();
	}

	nlohmann::json Serialize() override {
		return {
			{ "ComponentType", typeid(Camera).name() },
			{ "Yaw", yaw },
			{ "Pitch", pitch },
		}; 
	}

	void Deserialize(nlohmann::json json) override {
		yaw = json["Yaw"];
		pitch = json["Pitch"];

		UpdateVectors();

		MakeMainCamera();
	}

	void UpdateVectors() {

		glm::vec3 front_{
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch))
		};
		front = glm::normalize(front_);
		// also re-calculate the Right and Up vector
		right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = glm::normalize(glm::cross(right, front));
	}
private:
	glm::vec3 front{ .0f, .0f, 1.f };
	glm::vec3 up{ .0f, 1.f, .0f };
	glm::vec3 right{ 1.f, .0f, .0f };
	glm::vec3 worldUp{ .0f, 1.f, .0f };
	// euler Angles
	float yaw = -270.f;
	float pitch = .0f;
	// camera options
	float movementSpeed = 2.f;
	float mouseSensitivity = .0625f;
	float mouseSensitivityBoost = 2.f;

	void Move();

	static ComponentRegistrer<Camera> componentRegistrer; // static ensure registered at compile time
};