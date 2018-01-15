//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef TIMER_H
#define TIMER_H

#include "WindowsWrapper.h"

class timer
{
public:
	static void initTimer();

	// big four
	timer();
	timer(const timer &) = default;
	timer & operator= (const timer &) = default;
	~timer() = default;

	// methods
	void tic();
	void toc();
	void reset();
	float timeInSeconds();

private:	
	// methods
	LARGE_INTEGER getTimer();

	// data:
	static float	SecondsPerCycle;
	LARGE_INTEGER	ticTime;
	LARGE_INTEGER	tocTime;
	LARGE_INTEGER	deltaTime;
	float			timeSeconds;
	int				pad1;
};

#endif TIMER_H