/* 
* LedController.h
*
* Created: 12/1/2020 10:50:39 AM
* Author: AElmendo
*/


#ifndef __LEDCONTROLLER_H__
#define __LEDCONTROLLER_H__
#include <TBoard.h>
#include <LedDriver.h>
#include <RecipeAccess.h>
#include <Timer.h>
#include <stdbool.h>


typedef enum LEDSTATE {
	LEDDELAY,
	LEDON,
	LEDIDLE,
	LEDMANUAL
}LedState;

class LedController
{
//variables
private:
	Timer delayTimer;
	Timer runTimer;
	Timer manualPeriod;
	bool doneLatched;
	LedDriver ledDriver;
	uint8_t step;
	LedState state;
	LedRecipe ledRecipe;
	bool done;
	float manualCurrent;
//functions
public:
	LedController(const LedRecipe &recipe,volatile uint8_t* port,const uint8_t &pin);
	LedController();
	void Start();
	void StartManual();
	void Stop();
	void Pause();
	void UnPause();
	bool Check();
	bool IsDone();
	bool LedOn();
	void SetLatched();
	bool CheckLatched();
	~LedController();
protected:
private:
	//LedController(const LedController &c);
	//LedController& operator=( const LedController &c );

}; //LedController

#endif //__LEDCONTROLLER_H__
