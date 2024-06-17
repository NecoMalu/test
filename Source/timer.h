#pragma once
#include <windows.h>

class Timer {
public:
	Timer();
	~Timer();
	void Restart();
	double ElapsedTime();
private:
	LARGE_INTEGER freq;
	LARGE_INTEGER start;
};