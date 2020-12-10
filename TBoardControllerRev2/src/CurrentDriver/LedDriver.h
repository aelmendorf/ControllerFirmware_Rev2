/* 
* LedDriver.h
*
* Created: 12/1/2020 10:57:50 AM
* Author: AElmendo
*/


#ifndef __LEDDRIVER_H__
#define __LEDDRIVER_H__
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

class LedDriver
{
//variables
private:
	volatile uint8_t* port;
	uint8_t pin;
	float current;

//functions
public:
	LedDriver();
	LedDriver(volatile uint8_t* port,const uint8_t &pin,float current);
	LedDriver(volatile uint8_t* port,const uint8_t &pin);
	void Initialize();
	void TurnOn(bool reset);
	void TurnOn(const float &current,bool reset);
	void TurnOff();
	float GetCurrent();
	void SetCurrent(const float &current);
	~LedDriver();
	//LedDriver( const LedDriver &c );
	//LedDriver& operator=( const LedDriver &c );
private:


}; //LedDriver

#endif //__LEDDRIVER_H__
