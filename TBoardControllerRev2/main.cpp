/*
 * TBoardControllerRev2.cpp
 *
 * Created: 11/20/2020 10:49:21 AM
 * Author : AElmendo
 */ 

//

#include <TBoard.h>
//#include <BoardController.h>
#define INT_DECIMAL_STRING_SIZE(int_type) ((8*sizeof(int_type)-1)*10/33+3)

void TestSend();
void uprint(const char* str);
void WriteSettings();
void Setup();
void RunAllOff();
void RunRed();
void RunUVOff();
void RunUVOn();
void TestLEDs();

int main(void){
	Setup();
	RunUVOn();
	TestSend();
	//WriteSettings();
	return 1;
}

void TestLEDs(){
	RunRed();
}

void WriteSettings(){
	RunUVOn();
	_delay_ms(200);
	Recipe recipe;
	LedRecipe led1;
	led1.Led=LED1;
	led1.LedDelay=10;
	led1.LedCurrent=15;
	led1.LedRunTime=3600;
	
	LedRecipe led2;
	led2.Led=LED2;
	led2.LedDelay=50;
	led2.LedCurrent=20;
	led2.LedRunTime=2000;
	
	LedRecipe led3;
	led3.Led=LED3;
	led3.LedDelay=100;
	led3.LedCurrent=10;
	led3.LedRunTime=1000;
	recipe.SetLedRecipe(led1);
	recipe.SetLedRecipe(led2);
	recipe.SetLedRecipe(led3);
	
	EEPROM eeprom;
	eeprom.Initialize();
	eeprom.UpdateInMemoryBoardRecipe(recipe);
	eeprom.SaveRecipe();
	RunAllOff();
	RunRed();
	_delay_ms(500);
	RunUVOff();
}

void TestSend(){
	unsigned int value;
	EEPROM eeprom;
	eeprom.Initialize();
	m_usb_init();
	while(!m_usb_isconnected()); // wait for a connection
	while(1)
	{
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
					break;
				}
			}
			if(count>0){
				switch(buffer[0]){
					case 'r':{
						LedRecipe led1=eeprom.GetStoredLedRecipe(LED1);
						LedRecipe led2=eeprom.GetStoredLedRecipe(LED2);
						LedRecipe led3=eeprom.GetStoredLedRecipe(LED3);
						int size=2*INT_DECIMAL_STRING_SIZE(millis_t);
						size+=INT_DECIMAL_STRING_SIZE(uint8_t);
						size+=(int)3*sizeof(char);
						char outBuffer[size*3];
						sprintf(outBuffer,"%ld,%ld,%d;%ld,%ld,%d;%ld,%ld,%d;\r",
						led1.LedDelay,led1.LedRunTime,led1.LedCurrent,
						led2.LedDelay,led2.LedRunTime,led2.LedCurrent,
						led3.LedDelay,led3.LedRunTime,led3.LedCurrent);
						uprint(outBuffer);
						break;
					}
					case 'p':{
						//char buffer[bytesAvailable];
						//int count=0;
						//while(1){
						//char c=m_usb_rx_char();
						//if(c=='\r'){
						//break;
						//}else{
						//buffer[count]=c;
						//count++;
						//}
						//}
						LedRecipe led1;
						LedRecipe led2;
						LedRecipe led3;
						sscanf(buffer,"p;%ld,%ld,%d;%ld,%ld,%d;%ld,%ld,%d;",
						&led1.LedDelay,&led1.LedRunTime,&led1.LedCurrent,
						&led2.LedDelay,&led2.LedRunTime,&led2.LedCurrent,
						&led3.LedDelay,&led3.LedRunTime,&led3.LedCurrent);
						Recipe recipe;
						recipe.SetLedRecipe(led1);
						recipe.SetLedRecipe(led2);
						recipe.SetLedRecipe(led3);
						eeprom.UpdateInMemoryBoardRecipe(recipe);
						eeprom.SaveRecipe();
						uprint("s\r");
						break;
					}
					default:{
						break;
					}
				}
			}
		}
	}
}

void SplitBufferToArray(char *buffer, char * delim, char ** Output) {
	int partcount = 0;
	Output[partcount++] = buffer;
	char* ptr = buffer;
	while (ptr != 0) { //check if the string is over
		ptr = strstr(ptr, delim);
		if (ptr != NULL) {
			*ptr = 0;
			Output[partcount++] = ptr + strlen(delim);
			ptr = ptr + strlen(delim);
		}

	}
	Output[partcount++] = NULL;
}

void Setup(){
	////disable JTAG
	//MCUCR|=(1<<JTD);
	////_NOP();
	//MCUCR|=(1<<JTD);
	////Set clock 16mhz
	//CLKPR=0x80;
	//CLKPR=0;
		
	//Signal LEDS
	DDRB|=(1<<RED) | (1<<GREEN) | (1<<BLUE);
	PORTB|=(1<<RED) | (1<<GREEN) | (1<<BLUE);
		
	//Lid Switch setup
	PORTF|=(1<<LID_SWITCH);
	DDRF&=~(1<<LID_SWITCH);
}

void RunUVOff(){
	PORTB|=(1<<RED) | (1<<GREEN) | (1<<BLUE);
	PORTB&=~(1<<GREEN);
}

void RunUVOn(){
	PORTB|=(1<<RED) | (1<<GREEN) | (1<<BLUE);
	PORTB&=~(1<<BLUE);
}

void RunRed(){
	PORTB|=(1<<RED) | (1<<GREEN) | (1<<BLUE);
	PORTB&=~(1<<RED);
}

void RunAllOff(){
	PORTB|=(1<<RED) | (1<<GREEN) | (1<<BLUE);
}

void TestingUSB(){
	//const char* s="Hello this is a string\n";
	m_usb_init();
	while(!m_usb_isconnected());
	while(1){
		//int size=sizeof(s);
		//m_usb_tx_int(size);
		//m_usb_tx_char('\n');
		const char* s="";
		uprint("Hello this is a string\n");
		char buffer[250];
		int index=0;
		char c;
		_delay_ms(1000);
		do{
			if(m_usb_rx_available()){
				c=m_usb_rx_ascii();
				buffer[index++]=c;
			}
		}while(c!='\n');

		uprint("You entered: ");
		uprint(buffer);
	}

}

void uprint(const char* str){
	size_t size=strlen(str);
	for(int i=0;i<(int)size;i++){
		m_usb_tx_char(str[i]);
	}
}

int ParseDateTime(char index1,char index2){
	int digit1,digit2;
	digit1=index1-'0';
	digit2=index2-'0';
	if(digit1>0){
		return (digit1*10)+digit2;
		}else{
		return digit2;
	}
}
