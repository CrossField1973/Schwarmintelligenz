#include "timer.h"
#include <Windows.h>

Timer::Timer() : 
	startTime(0), currentTime(0), previousTime(0), secondsPerCount(0.0), deltaTime(0.0)
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
	__int64 now = 0;
	if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
	{
		startTime = now;
		previousTime = now;
		pausedTime = 0;
		isStopped = false;

		// return success
		return;
	}
}

void Timer::tick()
{
	if (isStopped)
	{
		// if the game is stopped, the elapsed time is obviously 0
		deltaTime = 0.0;

		// return success
		return;
	}
	else
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
			{
				deltaTime = 0.0;
			}
		}
	}	
}

void Timer::start()
{
	// this function starts the timer (if it is not already running)
	if (isStopped)
	{
		long long int now = 0;
		if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
		{
			// add the duration of the pause to the total idle time
			totalIdleTime += (now - pausedTime);

			// set the previous time to the current time
			previousTime = now;

			// reset the pausedTime to 0 and isStopped to false
			pausedTime = 0;
			isStopped = false;

			// return success
			return;
		}
	}

	// return success
	return;
}

void Timer::stop()
{
	// this function stops the timer (if it is currently running)
	if (!isStopped)
	{
		long long int now = 0;
		if (QueryPerformanceCounter((LARGE_INTEGER*)&now))
		{
			// set the time the timer was stopped to "now"
			pausedTime = now;
			isStopped = true;

			// return success
			return;
		}
	}

	// return success
	return;
}

double Timer::getDeltaTime() const
{
	// this function returns the time elapsed between two frames; delta time is updated during the game loop
	return deltaTime;
}

double Timer::getTotalTime() const
{
	// this function returns the total time since the game started: (t_now - t_start) - t_totalIdle
	if (isStopped)
		return (pausedTime - startTime - totalIdleTime) * secondsPerCount;
	else
		return (currentTime - startTime - totalIdleTime) * secondsPerCount;
}