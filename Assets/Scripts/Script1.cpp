#include "Engine/Core/Maths/trigonometric.hpp"

#include "Engine/Components/Script.hpp"
#include "Engine/Components/Transform.hpp"

#include "Engine/GameObject.hpp"
#include "Engine/Modules/Time.hpp"


class Script1 : public Script {
public:
	void Awake() override { }
	void Shutdown() override { }
	void Update() override {
		Transform()->rotation.z += Time::deltaTime * glm::radians(20.f);
	}

};


extern "C" Script* CreateScript() {
	return new Script1();
}