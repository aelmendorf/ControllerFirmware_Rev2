/* 
* LedDriver.cpp
*
* Created: 12/1/2020 10:57:50 AM
* Author: AElmendo
*/


#include "LedDriver.h"

LedDriver::LedDriver(){
	
}

// default constructor
LedDriver::LedDriver(volatile uint8_t* port,const uint8_t &pin,float current){
	this->port=port;
	this->pin=pin;
	this->current=current;
} //LedDriver

LedDriver::LedDriver(volatile uint8_t* port,const uint8_t &pin){
	this->port=port;
	this->pin=pin;
	this->current=0;
} //LedDriver

void LedDriver::Initialize(){
	*this->port&=~(1<<this->pin);
}

void LedDriver::TurnOn(bool reset){
	if(this->current<=0){
		*this->port&=~(1<<this->pin);
	}else{
		int strobes=33-((8*this->current)/5);
		if(reset){
			*this->port&=~(1<<pin);
			_delay_us(5);
		}
		for(int i=0;i<strobes;i++){
			*this->port|=(1<<this->pin);
			_delay_us(5);
			*this->port&=~(1<<this->pin);
		}
		_delay_us(5);
		*this->port|=(1<<this->pin);
		_delay_us(130);
	}//end if/else
}

void LedDriver::TurnOn(const float &current,bool reset){
	if(current<=0){
		*this->port&=~(1<<this->pin);
	}else{
		int strobes=33-((8*current)/5);
		if(reset){
			*this->port&=~(1<<pin);
			_delay_us(5);
		}
		for(int i=0;i<strobes;i++){
			*this->port|=(1<<this->pin);
			_delay_us(5);
			*this->port&=~(1<<this->pin);
		}
		_delay_us(5);
		*this->port|=(1<<this->pin);
		_delay_us(130);
	}//end if/else
}

float LedDriver::GetCurrent(){
	return this->current;
}

void LedDriver::TurnOff(){
	*this->port&=~(1<<this->pin);
}

void LedDriver::SetCurrent(const float &current){
	this->current=current;
}

// default destructor
LedDriver::~LedDriver(){
} //~LedDriver
