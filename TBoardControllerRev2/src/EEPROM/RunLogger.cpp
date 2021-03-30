/* 
* RunLogger.cpp
*
* Created: 12/2/2020 2:56:58 PM
* Author: AElmendo
*/


#include "RunLogger.h"
Log EEMEM eeprom_log;

// default constructor
RunLogger::RunLogger(){
	//this->log.BoardCycleCount=0;
	//this->log.LED1CycleCount=0;
	//this->log.LED2CycleCount=0;
	//this->log.LED3CycleCount=0;
} //RunLogger

int RunLogger::GetCycleCount(){
	return this->log.BoardCycleCount;
}

void RunLogger::Initialize(){
	eeprom_read_block((void*)&this->log,(const void*)&eeprom_log,sizeof(Log));
}

void RunLogger::ResetLog(){
	this->log.BoardCycleCount=0;
	this->log.LED1CycleCount=0;
	this->log.LED2CycleCount=0;
	this->log.LED3CycleCount=0;
	eeprom_write_block((const void*)&this->log,(void*)&eeprom_log,sizeof(Log));
}

Log RunLogger::GetInMemoryLog(){
	return this->log;
}

Log RunLogger::GetStoredLog(){
	eeprom_read_block((void*)&this->log,(const void*)&eeprom_log,sizeof(Log));
	return this->log;
}

void RunLogger::IncrementBoardCycle(){
	this->log.BoardCycleCount+=1;
	eeprom_update_block((const void*)&this->log.BoardCycleCount,(void*)&eeprom_log.BoardCycleCount,sizeof(uint8_t));
	//eeprom_write_block((const void*)&this->log,(void*)&eeprom_log,sizeof(Log));
}

void RunLogger::IncrementLedXCycle(int led){
	switch(led){
		case 0:{
			this->log.LED1CycleCount+=1;
			eeprom_update_block((const void*)&this->log.LED1CycleCount,(void*)&eeprom_log.LED1CycleCount,sizeof(uint8_t));
			break;
		}
		case 1:{
			this->log.LED2CycleCount+=1;
			eeprom_update_block((const void*)&this->log.LED2CycleCount,(void*)&eeprom_log.LED2CycleCount,sizeof(uint8_t));	
			break;
		}
		case 2:{
			this->log.LED3CycleCount+=1;
			eeprom_update_block((const void*)&this->log.LED3CycleCount,(void*)&eeprom_log.LED3CycleCount,sizeof(uint8_t));
			break;
		}
		default:{
			this->log.BoardCycleCount=55;
			this->log.LED1CycleCount=55;
			this->log.LED2CycleCount=55;
			this->log.LED3CycleCount=55;
			eeprom_write_block((const void*)&this->log,(void*)&eeprom_log,sizeof(Log));
			break;
		}
	}
	//eeprom_read_block((void*)&this->log,(const void*)&eeprom_log,sizeof(Log));
}

// default destructor
RunLogger::~RunLogger(){
	
} //~RunLogger
