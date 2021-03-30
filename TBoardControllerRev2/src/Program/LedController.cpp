/* 
* LedController.cpp
*
* Created: 12/1/2020 10:50:39 AM
* Author: AElmendo
*/


#include "LedController.h"

LedController::LedController(){
	this->state=LEDIDLE;
	this->done=false;
	this->doneLatched=false;
	this->manualCurrent=2;
}

LedController::LedController(const LedRecipe &recipe,volatile uint8_t* port,const uint8_t &pin){
	this->ledRecipe=recipe;
	this->delayTimer=Timer(MIN_TO_MS(recipe.LedDelay));
	this->runTimer=Timer(MIN_TO_MS(recipe.LedRunTime));
	//this->delayTimer=Timer(SEC_TO_MS(recipe.LedDelay));
	//this->runTimer=Timer(SEC_TO_MS(recipe.LedRunTime));
	this->manualPeriod=Timer(5000);
	this->ledDriver=LedDriver(port,pin,(float)this->ledRecipe.LedCurrent);
	this->state=LEDIDLE;
	this->done=false;
	this->doneLatched=false;
	this->manualCurrent=2;
}

void LedController::Start(){
	this->state=LEDDELAY;
	this->delayTimer.Reset();
	this->doneLatched=false;
	this->done=false;
}

bool LedController::Check(){
	bool running=false;
	switch(this->state){
		case LEDDELAY:{
			if(this->delayTimer.Done()){
				this->runTimer.Reset();
				this->ledDriver.TurnOn(false);
				this->state=LEDON;
				running=true;
			}
			break;
		}
		case LEDON:{
			if(this->runTimer.Done()){
				this->ledDriver.TurnOff();
				this->state=LEDIDLE;
				this->done=true;
			}
			break;
		}
		case LEDIDLE:{
			break;
		}
		case LEDMANUAL:{
			this->ledDriver.TurnOn(false);
			break;
		}
		default:
			break;
	}
	return running;
}

void LedController::StartManual(){
	this->state=LEDMANUAL;
	this->manualCurrent=2;
	this->manualPeriod.Reset();
	this->ledDriver.TurnOn(this->manualCurrent,false);
}

void LedController::Stop(){
	this->ledDriver.TurnOff();
	this->done=true;
	this->doneLatched=true;
}

bool LedController::IsDone(){
	return this->done;	
}

bool LedController::LedOn(){
	return this->state==LEDON;
}

void LedController::SetLatched(){
	this->doneLatched=true;
}

bool LedController::CheckLatched(){
	return this->doneLatched;
}

void LedController::Pause(){
	this->ledDriver.TurnOff();	
}

void LedController::UnPause(){
	this->ledDriver.TurnOn(false);
}

// default destructor
LedController::~LedController(){
	
} //~LedController
