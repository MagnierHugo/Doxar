#pragma once

struct PerformanceRecord {
	
public:
	PerformanceRecord();
	PerformanceRecord(double sampleDuration_);

	void Begin(double startTime);
	void End(double endTime);

	double Get() const;
private:
	int frameCount = 0;
	static const double MIN_SAMPLE_DURATION;
	double sampleDuration = MIN_SAMPLE_DURATION;
	double currentSampleDuration = .0f;
	double lastRecordedPerformance = 0;
	double currentPerformance = 0;
	double performanceAverage = 0;
};
