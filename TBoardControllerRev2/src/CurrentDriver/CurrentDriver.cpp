/* 
* CurrentDriver.cpp
*
* Created: 12/1/2020 9:58:10 AM
* Author: AElmendo
*/

#include "CurrentDriver.h"

// default constructor
CurrentDriver::CurrentDriver()
{
	this->ledCurrents[0]=0;
	this->ledCurrents[1]=1;
	this->ledCurrents[2]=2;
} //CurrentDriver

CurrentDriver::CurrentDriver(float led1Current,float led2Current,float led3Current)
{
	this->ledCurrents[0]=led1Current;
	this->ledCurrents[1]=led2Current;
	this->ledCurrents[2]=led3Current;
	
} //CurrentDriver

// default destructor
CurrentDriver::~CurrentDriver()
{

	
} //~CurrentDriver


void CurrentDriver::init(){
	//Current driver.  set output then pull low
	DDRC|=(1<<LED_CTRL1);
	DDRD|=(1<<LED_CTRL2) | (1<<LED_CTRL3);
	PORTC&=~(1<<LED_CTRL1);
	PORTD&=~(1<<LED_CTRL2);
	PORTD&=~(1<<LED_CTRL3);
}

void CurrentDriver::set_current(uint8_t led,uint8_t reset){
	volatile uint8_t *port;
	uint8_t pin;
	switch(led){
		case 0:{
			port=&PORTC;
			pin=LED_CTRL1;
			break;
		}
		case 1:{
			port=&PORTD;
			pin=LED_CTRL2;
			break;
		}
		case 2:{
			port=&PORTD;
			pin=LED_CTRL3;
			break;
		}
		default:{
			PORTC&=~(1<<LED_CTRL1);
			PORTD&=~(1<<LED_CTRL2);
			PORTD&=~(1<<LED_CTRL3);
			return;
		}
	}
	if(this->ledCurrents[led]<=0){
		*port&=~(1<<pin);
		}else{
		int strobes=33-((8*this->ledCurrents[led])/5);
		if(reset){
			*port&=~(1<<pin);
			_delay_us(5);
		}
		for(int i=0;i<strobes;i++){
			*port|=(1<<pin);
			_delay_us(5);
			*port&=~(1<<pin);
		}
		_delay_us(5);
		*port|=(1<<pin);
		_delay_us(130);
	}//end if/else
}

void CurrentDriver::set_current(uint8_t led,float current,uint8_t reset){
	volatile uint8_t *port;
	uint8_t pin;
	this->ledCurrents[led]=current;
	switch(led){
		case 0:{
			port=&PORTC;
			pin=LED_CTRL1;
			break;
		}
		case 1:{
			port=&PORTD;
			pin=LED_CTRL2;
			break;
		}
		case 2:{
			port=&PORTD;
			pin=LED_CTRL3;
			break;
		}
		default:{
			PORTC&=~(1<<LED_CTRL1);
			PORTD&=~(1<<LED_CTRL2);
			PORTD&=~(1<<LED_CTRL3);
			return;
		}
	}
	if(this->ledCurrents[led]<=0){
		*port&=~(1<<pin);
		}else{
		int strobes=33-((8*this->ledCurrents[led])/5);
		if(reset){
			*port&=~(1<<pin);
			_delay_us(5);
		}
		for(int i=0;i<strobes;i++){
			*port|=(1<<pin);
			_delay_us(5);
			*port&=~(1<<pin);
		}
		_delay_us(5);
		*port|=(1<<pin);
		_delay_us(130);
	}//end if/else
}//End set_current

void CurrentDriver::turn_off(uint8_t led){
	switch(led){
		case 0:{
			PORTC&=~(1<<LED_CTRL1);
			break;
		}
		case 1:{
			PORTD&=~(1<<LED_CTRL2);
			break;
		}
		case 2:{
			PORTD&=~(1<<LED_CTRL3);
			break;
		}
		default:{
			PORTC&=~(1<<LED_CTRL1);
			PORTD&=~(1<<LED_CTRL2);
			PORTD&=~(1<<LED_CTRL3);
			break;
		}
	}
	_delay_us(5);
}

void CurrentDriver::turn_on(uint8_t led){
	this->set_current(led,1);
}

void CurrentDriver::turn_offAll(){
	PORTC&=~(1<<LED_CTRL1);
	PORTD&=~(1<<LED_CTRL2);
	PORTD&=~(1<<LED_CTRL3);
}
