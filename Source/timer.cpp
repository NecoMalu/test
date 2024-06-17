#include "timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
}

Timer::~Timer()
{
}

void Timer::Restart()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
}

double Timer::ElapsedTime()
{
	LARGE_INTEGER cur;
	QueryPerformanceCounter(&cur);
	return static_cast<double>(cur.QuadPart - start.QuadPart) / freq.QuadPart;
}
