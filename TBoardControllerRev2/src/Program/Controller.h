/* 
* Controller.h
*
* Created: 12/1/2020 10:26:59 AM
* Author: Andrew Elmendorf
*/


#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include <TBoard.h>
#include <stdbool.h>
#include <Task.h>
#include <RunLogger.h>

typedef enum {
	TO,
	FROM
}Direction;

class Controller {
	
private:
	LedController ledControllers[LEDCOUNT];
	Switch switches[SWITCHCOUNT];
	RecipeAccess recipeAccess;
	RunLogger logger;
	Timer cycleTimer;
	BoardRecipe recipe;
	Task task,nextTask;
	bool switchLatch;
	
//functions
public:
	Controller();
	void Initialize();
	void Start();
	void Run();
	~Controller();
private:
	
	void DisplayUVOn();
	void DisplayUVOff();
	void DisplayError();
	void DisplayLED1On();
	void DisplayLED2On();
	void DisplayLED3On();
	void TurnAllLedOff();
	void TurnLEDDisplayOff();
	
	void Transition_Auto(Direction direction);
	void Transition_Wait(Direction direction);
	void Transition_USB(Direction direction);
	void TransitionManual_LED1(Direction direction);
	void TransitionManual_LED2(Direction direction);
	void TransitionManual_LED3(Direction direction);
	
	void RunAuto();
	void RunUSB();
	void RunManualLed1();
	void RunManualLed2();
	void RunManualLed3();
	
	void SetupPWM();
	bool CheckLid();
	int CheckMainSwitches();
	int CheckSecondarySwitches();
	void ProcessSwitch(int sw);
	void ProcessSecondarySwitch(int sw);
	
	void uprint(const char* str);
	
	typedef void (Controller::*ModePtr)(void);
	typedef void (Controller::*LedPtr)(void);
	typedef void (Controller::*TransitionPtr)(Direction);
	
	ModePtr mFunc[MODECOUNT];
	TransitionPtr tFunc[MODECOUNT];
	LedPtr lFunc[LEDCOUNT];
	
	Controller( const Controller &c );
	Controller& operator=( const Controller &c );

}; //Controller

#endif //__CONTROLLER_H__
