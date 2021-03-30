/*
 * TBoardControllerRev2.cpp
 *
 * Created: 11/20/2020 10:49:21 AM
 * Author : AElmendo
 */ 

//

#include <TBoard.h>
#include <Controller.h>
#include <string.h>
//#include <Run>
Controller controller;
void RunUVOn();
void RunUVOff();
void RunAllOff();
void WriteSettings();
void RunRed();
void Setup();
void ProcessSwitch(int sw);
int CheckMainSwitches();
void DisplayLed3On();
void WriteLogValues();
Switch switches[6];




int main(void){
	
	controller.Initialize();
	controller.Start();
	controller.Run();
	//Setup();
	//WriteLogValues();
	//WriteSettings();
	return 1;
}

int CheckMainSwitches(){
	for(int i=MAINSTART;i<MAINSTOP;i++){
		switches[i].debounce();
		if(switches[i].switched()){
			switches[i].clear_state();
			return i;
		}
		_delay_ms(SWITCHDELAY);
	}//end
	return -1;
}//End check mode

void ProcessSwitch(int sw){
	switch(sw){
		case POS1_B1:{
			RunUVOn();
			break;
		}
		case POS1_B2:{
			RunUVOff();
			break;
		}
		case POS1_B3:{
			RunRed();
			break;
		}
		default:{
			DisplayLed3On(); 
			break;
		}
	}
}

void WriteSettings(){
	RunUVOn();
	_delay_ms(200);
	Recipe recipe;
	recipe.CycleCount=2;
	recipe.CycleTime=MIN_TO_MS(1);
	LedRecipe led1;
	led1.Led=LED1;
	led1.LedDelay=SEC_TO_MS(1);
	led1.LedCurrent=15;
	led1.LedRunTime=SEC_TO_MS(5);
	
	LedRecipe led2;
	led2.Led=LED2;
	led2.LedDelay=SEC_TO_MS(3);
	led2.LedCurrent=10;
	led2.LedRunTime=SEC_TO_MS(5);
	
	LedRecipe led3;
	led3.Led=LED3;
	led3.LedDelay=SEC_TO_MS(5);
	led3.LedCurrent=5;
	led3.LedRunTime=SEC_TO_MS(5);
	recipe.SetLedRecipe(led1);
	recipe.SetLedRecipe(led2);
	recipe.SetLedRecipe(led3);
	
	RecipeAccess eeprom;
	//eeprom.Initialize();
	Log log;
	log.BoardCycleCount=1;
	log.LED1CycleCount=1;
	log.LED2CycleCount=2;
	log.LED3CycleCount=3;
	//RunLogger logger;
	//logger.Initialize();
	eeprom.UpdateInMemoryBoardRecipe(recipe);
	eeprom.SaveRecipe();
	eeprom_update_block((const void*)&log,(void*)&eeprom_log,sizeof(Log));
	RunAllOff();
	RunRed();
	_delay_ms(1000);
	RunUVOff();
}

void WriteLogValues(){
	RunUVOn();
	_delay_ms(200);
	RunLogger logger;
	logger.Initialize();
	logger.IncrementBoardCycle();
	logger.IncrementBoardCycle();
	logger.IncrementBoardCycle();
	logger.IncrementLedXCycle(0);
	logger.IncrementLedXCycle(1);
	logger.IncrementLedXCycle(2);
	logger.IncrementLedXCycle(0);
	logger.IncrementLedXCycle(1);
	logger.IncrementLedXCycle(2);
	logger.IncrementLedXCycle(0);
	logger.IncrementLedXCycle(1);
	logger.IncrementLedXCycle(2);
	RunAllOff();
	RunRed();
	_delay_ms(1000);
	RunUVOff();

}

//void WriteSettings(){
	//RunUVOn();
	//_delay_ms(200);
	//Recipe recipe;
	//recipe.CycleTime=MIN_TO_MS(1);
	//LedRecipe led1;
	//led1.Led=LED1;
	//led1.LedDelay=SEC_TO_MS(1);
	//led1.LedCurrent=15;
	//led1.LedRunTime=SEC_TO_MS(5);
	//
	//LedRecipe led2;
	//led2.Led=LED2;
	//led2.LedDelay=SEC_TO_MS(3);
	//led2.LedCurrent=10;
	//led2.LedRunTime=SEC_TO_MS(5);
	//
	//LedRecipe led3;
	//led3.Led=LED3;
	//led3.LedDelay=SEC_TO_MS(5);
	//led3.LedCurrent=5;
	//led3.LedRunTime=SEC_TO_MS(5);
	//recipe.SetLedRecipe(led1);
	//recipe.SetLedRecipe(led2);
	//recipe.SetLedRecipe(led3);
	//
	//RecipeAccess eeprom;
	//eeprom.Initialize();
	//eeprom.UpdateInMemoryBoardRecipe(recipe);
	//eeprom.SaveRecipe();
	//RunAllOff();
	//RunRed();
	//_delay_ms(500);
	//RunUVOff();
//}

void Setup(){
	//disable JTAG
	MCUCR|=(1<<JTD);
	//_NOP();
	MCUCR|=(1<<JTD);
	//Set clock 16mhz
	CLKPR=0x80;
	CLKPR=0;
	
	DDRC|=(1<<LED_CTRL1);
	DDRD|=(1<<LED_CTRL2) | (1<<LED_CTRL3);
	PORTC&=~(1<<LED_CTRL1);
	PORTD&=~(1<<LED_CTRL2);
	PORTD&=~(1<<LED_CTRL3);
	
	PORTB|=(1<<POS1_B1) |(1<<POS1_B2) |(1<<POS1_B3);//Enable pullup*/
	DDRB&=~(1<<POS1_B1) & ~(1<<POS1_B2) & ~(1<<POS1_B3);
		
	PORTD|=(1<<POS2_P1) |(1<<POS2_P2) |(1<<POS2_P3);//Enable pullup*/
	DDRD&=~(1<<POS2_P1) & ~(1<<POS2_P2) & ~(1<<POS2_P3);
	
	DDRB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	
	DDRD|=(1<<LEDG2) | (1<<LEDR2);
	PORTD|=(1<<LEDG2) | (1<<LEDR2);
	
	DDRC|=(1<<LEDB2);
	PORTC|=(1<<LEDB2);
	
	//Lid Switch setup
	PORTF|=(1<<LIDSW);
	DDRF&=~(1<<LIDSW);
}

void DisplayLed3On(){
	PORTD|=(1<<LEDR2) | (1<<LEDG2);
	PORTC|=(1<<LEDB2);
	PORTD&=~(1<<LEDR2);
}

void RunUVOff(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDG1);
}

void RunUVOn(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDB1);
}

void RunRed(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDR1);
}

void RunAllOff(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
}
