/* 
* Timers.h
*
* Created: 12/1/2020 10:30:09 AM
* Author: AElmendo
*/


#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdbool.h>
#include <millis.h>

class Timer
{
//variables
public:
protected:
private:
	millis_t last;
	millis_t now;
	millis_t wait;

//functions
public:
	Timer();
	Timer(const millis_t& waitTime);
	void SetWaitTime(const millis_t& waitTime);
	void Reset();
	millis_t Now();
	bool Done();
	~Timer();
	//Timer( const Timer &c );
	//Timer& operator=( const Timer &c );
}; //Timers

#endif //__TIMERS_H__
