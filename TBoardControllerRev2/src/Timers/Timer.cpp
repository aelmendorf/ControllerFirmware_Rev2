/* 
* Timers.cpp
*
* Created: 12/1/2020 10:30:09 AM
* Author: AElmendo
*/


#include "Timer.h"

Timer::Timer(){
	this->wait=0;
}

Timer::Timer(const millis_t& waitTime){
	this->wait=waitTime;
} //Timers

void Timer::SetWaitTime(const millis_t& waitTime){
	this->wait=waitTime;
}

void Timer::Reset(){
	this->last=this->Now();
}

bool Timer::Done(){
	return (this->Now()-this->last)>=this->wait;
}

millis_t Timer::Now(){
	this->now=millis_get();
	return this->now;
}

// default destructor
Timer::~Timer(){
	
} //~Timers
