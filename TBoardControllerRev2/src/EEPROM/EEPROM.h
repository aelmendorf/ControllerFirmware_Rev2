/* 
* EEPROM.h
*
* Created: 9/26/2018 1:05:44 PM
* Author: 600076
*/


#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <TBoard.h>
#include <avr/io.h>
#include <millis.h>
#include <avr/eeprom.h>


#define REPO_SIZE 3


typedef enum {LED1=1,LED2,LED3,NA}LED;

typedef struct LedRecipe{
	LED Led;
	millis_t LedDelay;
	millis_t LedRunTime;
	uint8_t LedCurrent;
	
	LedRecipe(){
		this->Led=LED1;
		this->LedDelay=1;
		this->LedRunTime=1;
		this->LedCurrent=1;
	}
	
	LedRecipe(LED led,millis_t delay,millis_t runtime,uint8_t current){
		this->Led=led;
		this->LedDelay=delay;
		this->LedRunTime=runtime;
		this->LedCurrent=current;
	}
	
	LedRecipe(const LedRecipe &c){
		this->Led=c.Led;
		this->LedDelay=c.LedDelay;
		this->LedRunTime=c.LedRunTime;
		this->LedCurrent=c.LedCurrent;
	}
	
	LedRecipe& operator=( const LedRecipe &c ){
		this->Led=c.Led;
		this->LedDelay=c.LedDelay;
		this->LedRunTime=c.LedRunTime;
		this->LedCurrent=c.LedCurrent;
		return *this;
	}
	
}LedRecipe;

typedef struct BoardRecipe{
	millis_t Led1Delay;
	millis_t Led1RunTime;
	uint8_t Led1Current;
	
	millis_t Led2Delay;
	millis_t Led2RunTime;
	uint8_t Led2Current;
	
	millis_t Led3Delay;
	millis_t Led3RunTime;
	uint8_t Led3Current;
	
	BoardRecipe(){
		
	}
	
	void SetLedRecipe(const LedRecipe &ledRecipe){
		switch(ledRecipe.Led){
			case LED1:{
				this->Led1Delay=ledRecipe.LedDelay;
				this->Led1RunTime=ledRecipe.LedRunTime;
				this->Led1Current=ledRecipe.LedCurrent;
				break;
			}
			case LED2:{
				this->Led2Delay=ledRecipe.LedDelay;
				this->Led2RunTime=ledRecipe.LedRunTime;
				this->Led2Current=ledRecipe.LedCurrent;
				break;
			}
			case LED3:{
				this->Led3Delay=ledRecipe.LedDelay;
				this->Led3RunTime=ledRecipe.LedRunTime;
				this->Led3Current=ledRecipe.LedCurrent;
				break;
			}
			default:{
				break;
			}
		}
	}
	
	BoardRecipe(const BoardRecipe &c){
		this->Led1Delay=c.Led1Delay;
		this->Led1RunTime=c.Led1RunTime;
		this->Led1Current=c.Led1Current;
		
		this->Led2Delay=c.Led2Delay;
		this->Led2RunTime=c.Led2RunTime;
		this->Led2Current=c.Led2Current;
		
		this->Led3Delay=c.Led3Delay;
		this->Led3RunTime=c.Led3RunTime;
		this->Led3Current=c.Led3Current;
	}
	
	BoardRecipe& operator=( const BoardRecipe &c ){
		this->Led1Delay=c.Led1Delay;
		this->Led1RunTime=c.Led1RunTime;
		this->Led1Current=c.Led1Current;
		
		this->Led2Delay=c.Led2Delay;
		this->Led2RunTime=c.Led2RunTime;
		this->Led2Current=c.Led2Current;
		
		this->Led3Delay=c.Led3Delay;
		this->Led3RunTime=c.Led3RunTime;
		this->Led3Current=c.Led3Current;
		return *this;
	}
	
	bool operator==( const BoardRecipe &c){
		bool equals=(c.Led1Current==this->Led1Current && 
					c.Led1Delay==this->Led1Delay &&
					c.Led1RunTime==this->Led1RunTime &&
					c.Led2Current==this->Led2Current &&
					c.Led2Delay==this->Led2Delay &&
					c.Led2RunTime==this->Led2RunTime &&
					c.Led3Current==this->Led3Current &&
					c.Led3Delay==this->Led3Delay &&
					c.Led3RunTime==this->Led3RunTime);
		return equals;
	}
	
}Recipe;

extern Recipe EEMEM eeprom_recipe;

class EEPROM
{
	private:
		Recipe recipe;
	//functions
	public:
		EEPROM();
		~EEPROM();
		void Initialize();
		LedRecipe GetStoredLedRecipe(const LED &led);
		LedRecipe GetInMemoryLedRecipe(const LED &led);
		Recipe GetInMemoryBoardRecipe();
		Recipe GetStoredBoardRecipe();
		void SaveRecipe();
		void UpdateInMemoryBoardRecipe(const Recipe &recipe);
		void UpdateInMemoryLedRecipe(const LedRecipe &ledRecipe);
	protected:
	private:
		bool CheckInMemoryRecipe();
		//EEPROM(const EEPROM &c);
		//EEPROM& operator=( const EEPROM &c );
};//EEPROM

#endif //__EEPROM_H__
