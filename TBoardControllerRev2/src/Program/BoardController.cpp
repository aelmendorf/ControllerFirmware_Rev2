/* 
* BoardController.cpp
*
* Created: 3/12/2019 12:41:52 PM
* Author: Andrew Elmendorf
* To Be Completely Redone
*/


#include "BoardController.h"

// default constructor
BoardController::BoardController()
{
	
	//all fields explicitly initialized in setup/start 
} //BoardController

void BoardController::Setup(){
	//disable JTAG
	MCUCR|=(1<<JTD);
	//_NOP();
	MCUCR|=(1<<JTD);
	//Set clock 16mhz
	CLKPR=0x80;
	CLKPR=0;
		
	//Signal LEDS
	DDRB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
		
	//Lid Switch setup
	PORTF|=(1<<LIDSW);
	DDRF&=~(1<<LIDSW);
	
	//Current Driver Ports
	DDRC|=(1<<LED_CTRL1);
	DDRD|=(1<<LED_CTRL2) | (1<<LED_CTRL3);
		
	//current driver setup
	this->currentDriver.init();
	//this->currentDriver.led1Current=SET_CURRENT;
}

void BoardController::Start(){
	this->Setup();
	millis_init();
	sei();
	this->time_reg.Init();
	this->task.state=INIT;
	this->task.error=false;
	this->switch_latch=false;
}

void BoardController::Run(){
	for(int i=0;i<BLINK;i++){
		RunRed();
		_delay_ms(250);
		RunAllOff();
		_delay_ms(250);
	}	
	while(1){
		switch(task.state){
			case INIT:{
				this->RunUVOff();
				this->time_reg.ResetDay();
				this->Transition_Wait(TO);
				//this->Transition_AlwaysOn(TO);
				break;
			}
			case WAIT_HR:{
				this->Wait_On();
				break;
			}
			case WAIT_DAY:{
				this->Wait_Cycle();
				break;
			}
			case WAIT_AUTO: {
				this->Wait_Auto();
				break;
			}
			case ALWAYS_ON:{
				this->Always_On();
				break;
			}
		}
		_delay_ms(5);
	}
}

bool BoardController::Check_Lid(){
	return (PINF & (1<<LIDSW));
}

void BoardController::Wait_Cycle(){
	if(this->Check_Lid()){
		if(!this->task.error){
			this->RunRed();
		}
		this->task.error=true;
	}else{
		if(this->task.error){
			this->RunUVOff();
		}
		this->task.error=false;
	}
	if(this->time_reg.DayDone()){
		this->Transition_Wait(TO);
	}
}

void BoardController::Wait_Auto(){
	if(this->Check_Lid()){
		if(!this->task.error){
			this->currentDriver.turn_off(0);
			this->RunRed();
		}
		this->task.error=true;
	}else{
		if(this->task.error){
			this->currentDriver.turn_on(0);
			this->RunUVOn();
		}
		this->task.error=false;
	}
	if(this->time_reg.AutoDone()){
		this->Transition_Auto(FROM);
	}
}

void BoardController::Wait_On(){
	if(this->Check_Lid()){
		if(!this->task.error){
			this->RunRed();
		}
		this->task.error=true;
	}else{
		if(this->task.error){
			this->RunUVOff();
		}
		this->task.error=false;
	}
	if(this->time_reg.HrDone()){
		this->Transition_Wait(FROM);
	}
}

void BoardController::Always_On(void){
	if(this->Check_Lid()){
		if(!this->task.error){
			this->RunRed();
			this->currentDriver.turn_off(1);
		}
		this->task.error=true;
	}else{
		if(this->task.error){
			this->RunUVOn();
			this->currentDriver.turn_on(1);
		}
		this->task.error=false;
	}
}

void BoardController::Transition_Auto(Direction direction){
	switch(direction){
		case TO:{
			if(!this->task.error){
				this->currentDriver.turn_on(1);
				this->RunUVOn();
			}
			this->task.state=WAIT_AUTO;
			this->time_reg.ResetAuto();
			break;
		}
		case FROM:{
			this->currentDriver.turn_off(1);
			if(!this->task.error){
				this->RunUVOff();
			}
			this->Transition_Cycle(TO);
			break;
		}
	}
}

void BoardController::Transition_Wait(Direction direction){
	switch(direction){
		case TO:{
			this->task.state=WAIT_HR;
			if(!this->task.error){
				this->RunUVOff();
			}
			this->time_reg.ResetDay();
			break;
		}
		case FROM:{
			this->Transition_Auto(TO);
			//this->Transition_AlwaysOn(TO);
			break;
		}
	}
}

void BoardController::Transition_Cycle(Direction direction){
	switch(direction){
		case TO:{
			this->task.state=WAIT_DAY;
			break;
		}
		case FROM:{
			this->time_reg.ResetDay();
			this->Transition_Wait(TO);
			break;
		}
	}
}

void BoardController::Transition_AlwaysOn(Direction direction){
	switch(direction){
		case TO:{
			this->task.state=ALWAYS_ON;
			this->currentDriver.turn_on(1);
			this->RunUVOn();
			break;
		}
		case FROM:{
			break;
		}
	}
}

void BoardController::RunUVOff(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDG1);
}

void BoardController::RunUVOn(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDB1);
}

void BoardController::RunRed(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDR1);
}

void BoardController::RunAllOff(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
}


// default destructor
BoardController::~BoardController()
{
	//no dynamic memory
} //~BoardController
