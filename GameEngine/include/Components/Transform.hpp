#pragma once

#include <iostream>


#include "Core/Maths/glm.hpp"
#include "Core/Maths/gtc/quaternion.hpp"
#include "Core/Maths/gtc/matrix_transform.hpp"
#include "Core/Maths/mat4x4.hpp"

#include "Component.hpp"

class Transform : public Component {
public:
	glm::vec3 position = glm::vec3(.0f, .0f, .0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::quat rotation = glm::quat(1.0f, .0f, .0f, .0f);

	inline glm::vec3 WorldToLocal(const glm::vec3& worldVector) const {
		return rotation * worldVector;
	}

	static inline glm::vec3 WorldToLocal(const glm::vec3& worldVector, const glm::quat& rotation) {
		return rotation * worldVector;
	}

	inline glm::mat4x4 GetMatrix() const {
		glm::mat4x4 scaleMatrix{ 1.f };
		scaleMatrix[0][0] = scale.x;
		scaleMatrix[1][1] = scale.y;
		scaleMatrix[2][2] = scale.z;

		glm::mat4x4 translationMatrix{ 1.f };
		translationMatrix[3][0] = position.x;
		translationMatrix[3][1] = position.y;
		translationMatrix[3][2] = position.z;

		return scaleMatrix * glm::mat4_cast(rotation) * translationMatrix;
	}

	void OnInspect() override {
		ImGUI::BeginChild("TransformComponent");

		if (ImGui::InputFloat3("Position", &position.x)) {
		}

		if (ImGui::InputFloat3("Scale", &scale.x)) {
		}

		// Rotation as Euler angles for easier editing
		glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));
		if (ImGui::InputFloat3("Rotation", &eulerAngles.x)) {
			rotation = glm::quat(glm::radians(eulerAngles));
		}

		ImGUI::EndChild();
	}

	nlohmann::json Serialize() override {
		return {
			{ "ComponentType", typeid(Transform).name()},
			{ "Position", { position.x, position.y, position.z } },
			{ "Scale", { scale.x, scale.y, scale.z } },
			{ "Rotation", { rotation.w,  rotation.x,  rotation.y,  rotation.z } },
		};
	}

	void Deserialize(nlohmann::json json) override {
		position = {
			json["Position"][0],
			json["Position"][1],
			json["Position"][2],
		};

		scale = {
			json["Scale"][0],
			json["Scale"][1],
			json["Scale"][2],
		};

		rotation = {
			json["Rotation"][0],
			json["Rotation"][1],
			json["Rotation"][2],
			json["Rotation"][3],
		};

		Owner->SetTransform(this);
	}

private:
	static ComponentRegistrer<Transform> componentRegistrer; // static ensure registered at compile time
};
