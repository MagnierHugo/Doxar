#include "Utilities/PerformanceRecord.hpp"
#include "Modules/Time.hpp"

const double PerformanceRecord::MIN_SAMPLE_DURATION = .1;

PerformanceRecord::PerformanceRecord() {
	sampleDuration = MIN_SAMPLE_DURATION;
}
PerformanceRecord::PerformanceRecord(double sampleDuration_) {
	sampleDuration = sampleDuration_ > MIN_SAMPLE_DURATION ? sampleDuration_ : MIN_SAMPLE_DURATION;
}

void PerformanceRecord::Begin(double startTime) {
	currentPerformance = startTime;
}

void PerformanceRecord::End(double endTime) {
	currentPerformance = (endTime - currentPerformance);
	performanceAverage += currentPerformance;

	frameCount++;
	currentSampleDuration += Time::deltaTime;
	if (currentSampleDuration >= sampleDuration) {

		lastRecordedPerformance = performanceAverage / frameCount;
		frameCount = 0;
		currentSampleDuration = .0;
		performanceAverage = .0;
	}
}

double PerformanceRecord::Get() const {
	return lastRecordedPerformance;
}