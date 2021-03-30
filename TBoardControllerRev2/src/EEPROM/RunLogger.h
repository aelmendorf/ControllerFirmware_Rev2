/* 
* RunLogger.h
*
* Created: 12/2/2020 2:56:58 PM
* Author: AElmendo
*/


#ifndef __RUNLOGGER_H__
#define __RUNLOGGER_H__

#include <avr/io.h>
#include <millis.h>
#include <avr/eeprom.h>

typedef struct Log{
	uint8_t BoardCycleCount;
	uint8_t LED1CycleCount;
	uint8_t LED2CycleCount;
	uint8_t LED3CycleCount;
	
	Log(){
		this->BoardCycleCount=0;
		this->LED1CycleCount=0;
		this->LED2CycleCount=0;
		this->LED3CycleCount=0;
	}
	
	Log(const Log &c){
		this->BoardCycleCount=c.BoardCycleCount;
		this->LED1CycleCount=c.LED1CycleCount;
		this->LED2CycleCount=c.LED2CycleCount;
		this->LED3CycleCount=c.LED3CycleCount;
	}
	
	Log& operator=(const Log &c){
		this->BoardCycleCount=c.BoardCycleCount;
		this->LED1CycleCount=c.LED1CycleCount;
		this->LED2CycleCount=c.LED2CycleCount;
		this->LED3CycleCount=c.LED3CycleCount;
		return *this;
	}
}Log;

extern Log EEMEM eeprom_log;

class RunLogger
{
//variables
private:
	Log log;
//functions
public:
	RunLogger();
	int	GetCycleCount();
	void Initialize();
	void ResetLog();
	Log GetInMemoryLog();
	Log GetStoredLog();
	void IncrementBoardCycle();
	void IncrementLedXCycle(int led);
	~RunLogger();
protected:
private:
	//RunLogger( const RunLogger &c );
	//RunLogger& operator=( const RunLogger &c );

}; //RunLogger

#endif //__RUNLOGGER_H__
