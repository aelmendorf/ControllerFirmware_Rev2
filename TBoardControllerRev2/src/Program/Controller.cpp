/* 
* Controller.cpp
*
* Created: 12/1/2020 10:26:59 AM
* Author: Andrew Elmendorf
*/


#include "Controller.h"

#pragma region Construct

// default constructor
Controller::Controller(){
	
} //Controller

// default destructor
Controller::~Controller(){
	
} //~Controller

#pragma endregion Construct


#pragma region Main 

void Controller::Initialize(){
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
	
	DDRD|=(1<<LEDG2) | (1<<LEDR2);
	PORTD|=(1<<LEDG2) | (1<<LEDR2);
	
	DDRC|=(1<<LEDB2);
	PORTC|=(1<<LEDB2);
	
	//Lid Switch setup
	PORTF|=(1<<LIDSW);
	DDRF&=~(1<<LIDSW);
	
	//Current Driver Ports
	DDRC|=(1<<LED_CTRL1);
	DDRD|=(1<<LED_CTRL2) | (1<<LED_CTRL3);
	
	PORTB|=(1<<POS1_B1) |(1<<POS1_B2) |(1<<POS1_B3);//Enable pullup*/
	DDRB&=~(1<<POS1_B1) & ~(1<<POS1_B1) & ~(1<<POS1_B1);
	
	PORTD|=(1<<POS2_P1) |(1<<POS2_P2) |(1<<POS2_P3);//Enable pullup*/
	DDRD&=~(1<<POS2_P1) & ~(1<<POS2_P2) & ~(1<<POS2_P3);
	
	sei();
	this->task.error=false;
	this->task.state=RUNNING;
	this->nextTask=this->task;
	this->recipeAccess.Initialize();
	this->logger.Initialize();
	this->recipe=this->recipeAccess.GetInMemoryBoardRecipe();
	//this->cycleTimer=Timer(MIN_TO_MS(this->recipe.CycleTime));
	this->cycleTimer=Timer(HOUR_TO_MS(this->recipe.CycleTime));
	this->ledControllers[LED1Index]=LedController(this->recipe.GetLedRecipe(LED1),&PORTC,LED_CTRL1);
	this->ledControllers[LED2Index]=LedController(this->recipe.GetLedRecipe(LED2),&PORTD,LED_CTRL2);
	this->ledControllers[LED3Index]=LedController(this->recipe.GetLedRecipe(LED3),&PORTD,LED_CTRL3);
	
	this->switches[0]=Switch(&PINB,POS1_B1,LOW);
	this->switches[1]=Switch(&PINB,POS1_B2,LOW);
	this->switches[2]=Switch(&PINB,POS1_B3,LOW);
	this->switches[3]=Switch(&PIND,POS2_P1,LOW);
	this->switches[4]=Switch(&PIND,POS2_P2,LOW);
	this->switches[5]=Switch(&PIND,POS2_P3,LOW);
	
	this->mFunc[AUTO]=&Controller::RunAuto;
	this->mFunc[USB]=&Controller::RunUSB;
	this->mFunc[MANUAL_L1]=&Controller::RunManualLed1;
	this->mFunc[MANUAL_L2]=&Controller::RunManualLed2;
	this->mFunc[MANUAL_L3]=&Controller::RunManualLed3;
	
	this->tFunc[AUTO]=&Controller::Transition_Auto;
	this->tFunc[USB]=&Controller::Transition_USB;
	this->tFunc[MANUAL_L1]=&Controller::TransitionManual_LED1;
	this->tFunc[MANUAL_L2]=&Controller::TransitionManual_LED2;
	this->tFunc[MANUAL_L3]=&Controller::TransitionManual_LED3;
	
	this->lFunc[LED1Index]=&Controller::DisplayLED1On;
	this->lFunc[LED2Index]=&Controller::DisplayLED2On;
	this->lFunc[LED3Index]=&Controller::DisplayLED3On;
	
	m_usb_init();
	millis_init();
}

void Controller::Start(){
	int sw=this->CheckMainSwitches();
	this->ProcessSwitch(sw);
	this->task=this->nextTask;
	this->task.latched=0;
	//(this->*mFunc[this->task.mode])();
}

void Controller::Run(){
	while(1){
		int sw=this->CheckMainSwitches();
		this->ProcessSwitch(sw);
		if(this->task!=this->nextTask){
			(this->*tFunc[this->task.mode])(FROM);
		}else{
			(this->*mFunc[this->task.mode])();
		}
	}
}

bool Controller::CheckLid(){
	return (PINF & (1<<LIDSW));
}

int Controller::CheckMainSwitches(){
	for(int i=MAINSTART;i<MAINSTOP;i++){
		this->switches[i].debounce();
		if(this->switches[i].switched()){
			this->switches[i].clear_state();
			return i;
		}
		_delay_ms(SWITCHDELAY);
	}//end
	return -1;
}//End check mode

int Controller::CheckSecondarySwitches(){
	for(int i=SECONDSTART;i<SECONDSTOP;i++){
		this->switches[i].debounce();
		if(this->switches[i].switched()){
			this->switches[i].clear_state();
			return i;
		}
		_delay_ms(SWITCHDELAY);
	}//end
	return -1;
}//End check mode

void Controller::ProcessSwitch(int sw){
	switch(sw){
		case POS1_B1:{
			this->nextTask.mode=AUTO;
			this->nextTask.state=RUNNING;
			this->nextTask.latched=false;
			this->nextTask.error=false;
			break;
		}
		case POS1_B2:{
			this->nextTask.mode=USB;
			this->nextTask.state=RUNNING;
			this->nextTask.latched=false;
			this->nextTask.error=false;
			break;
		}
		case POS1_B3:{
			int s_sw=this->CheckSecondarySwitches();
			this->ProcessSecondarySwitch(s_sw);
			break;
		}
		default:{
			//this->nextTask.mode=NOMODE;
			//this->nextTask.state=RUNNING;
			//this->nextTask.latched=false;
			//this->nextTask.error=false;
			break;	
		}
	}
}

void Controller::ProcessSecondarySwitch(int sw){
		switch(sw){
			case POS2_B1:{
				this->nextTask.mode=MANUAL_L1;
				this->nextTask.state=RUNNING;
				this->nextTask.latched=false;
				this->nextTask.error=false;
				break;
			}
			case POS2_B2:{
				this->nextTask.mode=MANUAL_L2;
				this->nextTask.state=RUNNING;
				this->nextTask.latched=false;
				this->nextTask.error=false;
				break;
			}
			case POS2_B3:{					
				this->nextTask.mode=MANUAL_L3;
				this->nextTask.state=RUNNING;
				this->nextTask.latched=false;
				this->nextTask.error=false;
				break;
			}
			default:{
				//this->nextTask.mode=NOMODE;
				//this->nextTask.state=RUNNING;
				//this->nextTask.latched=false;
				//this->nextTask.error=false;
				break;
			}
		}
}

#pragma endregion Main

#pragma region Modes

void Controller::RunAuto(){
	switch(this->task.state){
		case RUNNING:{
			if(this->CheckLid()){
				if(!this->task.error){
					this->task.error=true;
					this->DisplayError();
					for(int i=0;i<LEDCOUNT;i++){
						this->ledControllers[i].Pause();
					}
				}
			}else{
				if(this->task.error){
					this->DisplayUVOn();
					this->task.error=false;
					for(int i=0;i<LEDCOUNT;i++){
						if(this->ledControllers[i].LedOn()){
							this->ledControllers[i].UnPause();
						}
					}
				}
				int doneCount=0;
				for(int i=0;i<LEDCOUNT;i++){
					if(this->ledControllers[i].Check()){
						(this->*lFunc[i])();
					}
					if(this->ledControllers[i].IsDone()){
						doneCount++;
						if(!this->ledControllers[i].CheckLatched()){
							this->ledControllers[i].SetLatched();
							this->logger.IncrementLedXCycle(i);
						}
					}
				}
				if(doneCount==LEDCOUNT){
					this->task.state=IDLE;
					this->DisplayUVOff();
					this->TurnLEDDisplayOff();
				}
			}
			

			break;
		}
		case IDLE:{
			if(this->cycleTimer.Done()){
				int cycleCount=this->logger.GetCycleCount();
				if(!(cycleCount>=this->recipe.CycleCount)){
					this->logger.IncrementBoardCycle();
					this->cycleTimer.Reset();
					this->task.state=RUNNING;
					this->DisplayUVOn();
					for(int i=0;i<LEDCOUNT;i++){
						this->ledControllers[i].Start();
					}
				}
			}
			break;
		}
		case ERROR:{
			break;
		}
		case NOSTATE:{
			break;
		}
	}
}

void Controller::RunUSB(){
#if USB_ENABLE

	while(!m_usb_isconnected()){
		this->switches[USBSW].debounce();
		if(!this->switches[USBSW].switched()){
			return;
		}
	}
	this->DisplayLED3On();
	while(1)
	{
		this->switches[USBSW].debounce();
		if(!this->switches[USBSW].switched()){
			return;
		}
		char bytesAvailable=m_usb_rx_available();
		if(bytesAvailable)
		{
			char buffer[bytesAvailable];
			int count=0;
			while(1){
				char in=m_usb_rx_char();
				if(in!='\r'){
					if(count>bytesAvailable){
						break;
					}else{
						buffer[count]=in;
						count++;
					}
				}else{
					//buffer[count]=in;
					break;
				}
			}
			if(count>0){
				switch(buffer[0]){
					case 'r':{
						this->recipe=this->recipeAccess.GetInMemoryBoardRecipe();						
						int size=2*INT_DECIMAL_STRING_SIZE(millis_t);//delay and runtime
						size+=INT_DECIMAL_STRING_SIZE(int);//current
						size+=(int)3*sizeof(char);//term
						size=size*3;//led*3
						size+=INT_DECIMAL_STRING_SIZE(millis_t);//cycle time
						size+=INT_DECIMAL_STRING_SIZE(int);//cycle count
						char outBuffer[size];
						//int size=timerSize+iSize+cSize;
						//char outBuffer[size];
						sprintf(outBuffer,"%d,%ld;%ld,%ld,%d;%ld,%ld,%d;%ld,%ld,%d;\r",
						recipe.CycleCount,recipe.CycleTime,recipe.Led1Delay,recipe.Led1RunTime,
						recipe.Led1Current,recipe.Led2Delay,recipe.Led2RunTime,
						recipe.Led2Current,recipe.Led3Delay,recipe.Led3RunTime,recipe.Led3Current);
						uprint(outBuffer);
						break;
					}
					case 'p':{
						Recipe input;
						char* token = strtok(buffer, ";");
						int tCount=0;
						while( token != NULL ) {
							if(strcmp(token,"p")!=0){
								if(tCount==0){
									sscanf(token,"%d,%ld",&recipe.CycleCount,&recipe.CycleTime);
								}else if(tCount==1){
									sscanf(token,"%ld,%ld,%d",&recipe.Led1Delay,&recipe.Led1RunTime,&recipe.Led1Current);
								}else if(tCount==2){
									sscanf(token,"%ld,%ld,%d",&recipe.Led2Delay,&recipe.Led2RunTime,&recipe.Led2Current);
								}else if(tCount==3){
									sscanf(token,"%ld,%ld,%d",&recipe.Led3Delay,&recipe.Led3RunTime,&recipe.Led3Current);
								}
								tCount+=1;
							}
							token = strtok(NULL, ";");
						}
						recipeAccess.UpdateInMemoryBoardRecipe(recipe);
						recipeAccess.SaveRecipe();						
						int size=2*INT_DECIMAL_STRING_SIZE(millis_t);//delay and runtime
						size+=INT_DECIMAL_STRING_SIZE(int);//current
						size+=(int)3*sizeof(char);//term
						size=size*3;//led*3
						size+=INT_DECIMAL_STRING_SIZE(millis_t);//cycle time
						size+=INT_DECIMAL_STRING_SIZE(int);//cycle count
						char outBuffer[size];
						sprintf(outBuffer,"s;%d,%ld;%ld,%ld,%d;%ld,%ld,%d;%ld,%ld,%d;\r",
						recipe.CycleCount,recipe.CycleTime,recipe.Led1Delay,recipe.Led1RunTime,
						recipe.Led1Current,recipe.Led2Delay,recipe.Led2RunTime,
						recipe.Led2Current,recipe.Led3Delay,recipe.Led3RunTime,recipe.Led3Current);	
						uprint(outBuffer);
						//uprint("s;\r");
						break;
					}
					case 'l':{
						Log log=this->logger.GetStoredLog();
						int size=4*INT_DECIMAL_STRING_SIZE(uint8_t);
						char outBuffer[size];
						sprintf(outBuffer,"l;%d,%d,%d,%d;\r",
						log.BoardCycleCount,
						log.LED1CycleCount,log.LED2CycleCount,log.LED3CycleCount);
						uprint(outBuffer);
						break;
					}
					default:{
						break;
					}
				}
			}
		}
	}
#endif

}

void Controller::RunManualLed1(){
	this->ledControllers[LED1Index].Check();
}

void Controller::RunManualLed2(){
	this->ledControllers[LED2Index].Check();
}

void Controller::RunManualLed3(){
	this->ledControllers[LED3Index].Check();
}

#pragma endregion Modes

#pragma region TransitionMode

void Controller::Transition_Auto(Direction direction){
	switch(direction){
		case TO:{
			this->task=this->nextTask;
			this->task.latched=true;
			this->task.error=false;
			this->task.state=RUNNING;
			this->cycleTimer.Reset();
			this->logger.ResetLog();
			this->DisplayUVOn();
			_delay_ms(200);
			this->TurnAllLedOff();
			_delay_ms(200);
			this->DisplayUVOn();
			for(int i=0;i<LEDCOUNT;i++){
				this->ledControllers[i].Start();
			}
			(this->*mFunc[this->task.mode])();
			break;
		}
		case FROM:{
			for(int i=0;i<LEDCOUNT;i++){
				this->ledControllers[i].Stop();
			}
			this->DisplayUVOff();
			this->TurnLEDDisplayOff();
			(this->*tFunc[this->nextTask.mode])(TO);
			break;
		}
		default:
			break;
	}

}

void Controller::Transition_USB(Direction direction){
	switch(direction){
		case TO:{
			this->task=this->nextTask;
			this->task.latched=true;
			this->task.error=false;
			m_usb_init();
			this->DisplayUVOff();
			//this->DisplayLED3On();
			(this->*mFunc[this->task.mode])();
			break;
		}
		case FROM:{
			this->TurnAllLedOff();
			this->TurnLEDDisplayOff();
			(this->*tFunc[this->nextTask.mode])(TO);
			break;
		}
		default:
			break;
	}
}

void Controller::TransitionManual_LED1(Direction direction){
	switch(direction){
		case TO:{
			this->task=this->nextTask;
			this->task.latched=true;
			this->task.error=false;
			this->DisplayUVOn();
			this->DisplayLED1On();
			this->ledControllers[LED1Index].StartManual();
			(this->*mFunc[this->task.mode])();
			break;
		}
		case FROM:{
			this->ledControllers[LED1Index].Stop();
			this->DisplayUVOff();
			this->TurnLEDDisplayOff();
			(this->*tFunc[this->nextTask.mode])(TO);
			break;
		}
		default:
		break;
	}
}

void Controller::TransitionManual_LED2(Direction direction){
	switch(direction){
		case TO:{
			this->task=this->nextTask;
			this->task.latched=true;
			this->task.error=false;
			this->DisplayUVOn();
			this->DisplayLED2On();
			this->ledControllers[LED2Index].StartManual();
			(this->*mFunc[this->task.mode])();
			break;
		}
		case FROM:{
			this->ledControllers[LED2Index].Stop();
			this->DisplayUVOff();
			this->TurnLEDDisplayOff();
			(this->*tFunc[this->nextTask.mode])(TO);
			break;
		}
		default:
		break;
	}
}

void Controller::TransitionManual_LED3(Direction direction){
	switch(direction){
		case TO:{
			this->task=this->nextTask;
			this->task.latched=true;
			this->task.error=false;
			this->DisplayUVOn();
			this->DisplayLED3On();
			this->ledControllers[LED3Index].StartManual();
			(this->*mFunc[this->task.mode])();
			break;
		}
		case FROM:{
			this->ledControllers[LED3Index].Stop();
			this->DisplayUVOff();
			this->TurnLEDDisplayOff();
			(this->*tFunc[this->nextTask.mode])(TO);
			break;
		}
		default:
		break;
	}
}

#pragma endregion TransitionMode

#pragma region Display

void Controller::DisplayUVOn(){
	PORTD|=(1<<LEDR2) | (1<<LEDG2);
	PORTC|=(1<<LEDB2);
	PORTC&=~(1<<LEDB2);
}

void Controller::DisplayUVOff(){
	PORTD|=(1<<LEDR2) | (1<<LEDG2);
	PORTC|=(1<<LEDB2);
	PORTD&=~(1<<LEDG2);
}

void Controller::DisplayError(){
	PORTD|=(1<<LEDR2) | (1<<LEDG2);
	PORTC|=(1<<LEDB2);
	PORTD&=~(1<<LEDR2);
}

void Controller::DisplayLED1On(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDB1);
}

void Controller::DisplayLED2On(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDG1);
}

void Controller::DisplayLED3On(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTB&=~(1<<LEDR1);
}

void Controller::TurnAllLedOff(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
	PORTD|=(1<<LEDR2) | (1<<LEDG2);
	PORTC|=(1<<LEDB2);
}

void Controller::TurnLEDDisplayOff(){
	PORTB|=(1<<LEDR1) | (1<<LEDG1) | (1<<LEDB1);
}

void Controller::uprint(const char* str){
	size_t size=strlen(str);
	for(int i=0;i<(int)size;i++){
		m_usb_tx_char(str[i]);
	}
}

#pragma endregion Display




