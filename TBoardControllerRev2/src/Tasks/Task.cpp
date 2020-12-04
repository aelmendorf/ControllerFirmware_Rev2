/* 
* Task.cpp
*
* Created: 2/19/2019 10:27:00 AM
* Author: Andrew Elmendorf
*/


#include "Task.h"

// default constructor
Task::Task(){
	this->latched=false;
	this->error=false;
	this->state=NOSTATE;
} //Task

void Task::Clear(){
	this->latched=false;
	this->state=NOSTATE;
	this->mode=NOMODE;
}

State Task::GetState(){
	return this->state;
}

Task::Task(const Task &c){
	this->error=c.error;
	this->state=c.state;
	this->mode=c.mode;
}

Task& Task::operator=(const Task &rhs){
	this->error=rhs.error;
	this->state=rhs.state;
	this->mode=rhs.mode;
	return *this;
}

bool Task::operator !=(const Task &rhs)const{
	return (this->mode!=rhs.mode);
}

bool Task::operator==(const Task &rhs)const{
	return (this->mode==rhs.mode);
}

// default destructor
Task::~Task(){
} //~Task
