#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>


#include "Modules/Time.hpp"
#include "Utilities/PerformanceRecord.hpp"

float Time::deltaTime = .0;
float Time::time = .0;
Time* Time::instance = nullptr;

Time::Time() {
	instance = this;

	if (!glfwInit()) {

		std::cerr << "No GLFW for u ^^" << std::endl;
		return;
	}
}

void Time::Init() {
	deltaTime = .0f;
	time = .0f;
	glfwSetTime(.0);
}

void Time::Update() {
	deltaTime = (float)glfwGetTime() - time;
	time += deltaTime;

	// ImGui::GLWFiguess::Update(<someParam>); // can t be bothered with searching for it RN
	
	currentFrameRateSampleCounter += deltaTime;
	frameCount++;
	if (currentFrameRateSampleCounter > frameRateSampleDuration) {

	 	lastRecordedFps = 1 / (currentFrameRateSampleCounter / frameCount);
	 	currentFrameRateSampleCounter = 0;
	 	frameCount = 0;
	}
	
	// ImGUI::Begin("FPS");
	// ImGUI::SetNextItemWidth(50.0f);
	// ImGUI::InputFloat(
	// 	"FPS sampling period", &frameRateSampleDuration,
	// 	.0f, .0f, "%.1fs"
	// );
	// frameRateSampleDuration = frameRateSampleDuration < .1f ? .01f : frameRateSampleDuration;
	// ImGUI::Text("%.0f FPS over last %.1fs", lastRecordedFps, frameRateSampleDuration);
	// ImGUI::End();
}

#ifdef DEBUG
void Time::BeginTimedExecution(const std::string& name) {

	instance->timedExecutions[name].Begin(glfwGetTime());
}

void Time::TerminateTimedExecution(const std::string& name) {

	instance->timedExecutions[name].End(glfwGetTime());
	const std::string& title = "[TimeIt]: " + name;

	std::cout << title << ": " << instance->timedExecutions[name].Get() << std::endl;
	
	// ImGUI::Begin(title.c_str());
	// ImGUI::Text("%ds", instance->timedExecutions[name].Get());
	// ImGUI::End();
}
#endif
