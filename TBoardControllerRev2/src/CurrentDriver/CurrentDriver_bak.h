/* 
* CurrentDriver.h
*
* Created: 9/19/2018 10:58:12 AM
* Author: Andrew Elmendorf
*/


#ifndef __CURRENTDRIVER_H__
#define __CURRENTDRIVER_H__
#include <TBoard.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

class CurrentDriver_back
{
//variables
public:
	float ledCurrents[3];
protected:
private:

//functions
public:
	CurrentDriver_back();
	CurrentDriver_back(float led1Current,float led2Current,float led3Current);
	~CurrentDriver_back();
	void turn_off(uint8_t led);
	void turn_on(uint8_t led);
	void turn_offAll();
	void init(void);
	void set_current(uint8_t led,uint8_t reset);
	void set_current(uint8_t led,float current,uint8_t reset);
	uint8_t get_display(void);
	int8_t set_current_value(uint8_t value);
protected:
private:

}; //CurrentDriver

#endif //__CURRENTDRIVER_H__
