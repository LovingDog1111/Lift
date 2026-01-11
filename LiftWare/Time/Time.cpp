#include "Time.h"
#include <Windows.h>

unsigned __int64 Time::getCurrentMs() {
	FILETIME f;
	GetSystemTimeAsFileTime(&f);
	(long long)f.dwHighDateTime;
	unsigned __int64 nano = ((__int64)f.dwHighDateTime << 32LL) + (__int64)f.dwLowDateTime;
	return (nano - 116444736000000000LL) / 10000;
}

float currentMs = Time::getTime();
float Time::getTime() {
	std::chrono::high_resolution_clock m_clock;
	return std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count() / 1000000.f;
}

void Time::reset() {
	currentMs = getTime();
}

bool Time::hasTimeElapsed(float sec) {
	if (getTime() - currentMs >= (sec)) {
		reset();
		return true;
	}
	return false;
}