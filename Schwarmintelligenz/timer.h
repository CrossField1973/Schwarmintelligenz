#pragma once

class Timer
{
private:
	// times measured in counts
	long long int startTime;			// time at the start of the application
	long long int currentTime;			// stores the current time; i.e. time at the current frame
	long long int previousTime;		    // stores the time at the last inquiry before current; i.e. time at the previous frame

	// times measured in seconds
	double secondsPerCount;			    // reciprocal of the frequency, computed once at the initialization of the class
	double deltaTime;					// time between two frames, updated during the game loop

public:
	// constructor
	Timer();

	// getters: return time measured in seconds
	double getTotalTime() const;		// returns the total time the game has been running (minus paused time)
	double getDeltaTime() const;		// returns the time between two frames

	// methods
	void reset();		// sets the counter to zero, called once before message loop
	void tick();		// called every frame, lets the time tick
};