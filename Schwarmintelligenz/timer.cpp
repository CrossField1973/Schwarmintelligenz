#include "timer.h"
#include <Windows.h>

Timer::Timer() : startTime(0), currentTime(0), previousTime(0), secondsPerCount(0.0), deltaTime(0.0)
{
	// get the frequency of the PerformanceCounter
	long long int frequency = 0;
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
	{
		// compute the secondsPerCount as the reciprocal of the frequency
		secondsPerCount = 1.0 / (double)frequency;
	}
}


void Timer::reset()
{
	// this function resets the timer
	long long int now = 0;
	if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
	{
		startTime = now;
		previousTime = now;

		// return success
		return;
	}
}

void Timer::tick()
{
	// get the current time
	if (QueryPerformanceCounter((LARGE_INTEGER*)&currentTime))
	{
		// compute the time elapsed since the previous frame
		deltaTime = (currentTime - previousTime) * secondsPerCount;

		// set previousTime to crrentTime, as in the next tick, this frame will be the previous frame
		previousTime = currentTime;

		// deltaTime can be negative if the processor goes idle for example
		if (deltaTime < 0.0)
			deltaTime = 0.0;

		// return success
		return;
	}
}

double Timer::getDeltaTime() const
{
	// this function returns the time elapsed between two frames; delta time is updated during the game loop
	return deltaTime;
}

double Timer::getTotalTime() const
{
	// this function returns the total time since the game started: (t_now - t_start) - t_totalIdle
	return (currentTime - startTime) * secondsPerCount;
}