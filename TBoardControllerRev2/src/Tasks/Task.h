/* 
* Task.h
*
* Created: 2/19/2019 10:27:00 AM
* Author: Andrew Elmendorf
*/


#ifndef __TASK_H__
#define __TASK_H__
#include <avr/io.h>

//typedef enum {
	//INIT,WAIT_HR,WAIT_DAY,WAIT_AUTO,ALWAYS_ON
//}State;

typedef enum{
	RUNNING,
	WAIT,
	ERROR,//
	IDLE,
	NOSTATE
}State;

typedef enum {
	AUTO,
	USB,
	MANUAL_L1,
	MANUAL_L2,
	MANUAL_L3,
	NOMODE
}Mode;

class Task
{
//variables
public:
	bool  error;
	State state;
	Mode mode;
	bool latched;
protected:
private:

//functions
public:
	Task();
	void Clear();
	State GetState();
	Task(const Task &c);
	Task& operator=(const Task &c);
	bool operator!=(const Task &c)const;
	bool operator==(const Task &c)const;
	~Task();
protected:
private:


}; //Task

#endif //__TASK_H__
