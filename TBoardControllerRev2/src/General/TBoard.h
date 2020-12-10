/*
 * TBoard.h
 *
 * Created: 2/18/2019 10:15:46 AM
 *  Author: Andrew Elmendorf
 */ 

#ifndef TBOARD_H_
#define TBOARD_H_
#define F_CPU 16000000UL
#include <stddef.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <CurrentDriver.h>
#include <RecipeAccess.h>
#include <millis.h>
#include <Switch.h>
#include <Timer.h>
#include <LedDriver.h>
#include <LedController.h>
#include "m_general.h"
#include "m_usb.h"

#define USB_ENABLE		1
#define DEBUG			0

#define TRUE			(uint8_t)1
#define FALSE			(uint8_t)0

#define HIGH			(uint8_t)1
#define LOW				(uint8_t)0

//Ports
#define LEDR1			PORTB4
#define LEDB1			PORTB6
#define LEDG1			PORTB5

#define LEDR2			PORTD6
#define LEDB2			PORTC6
#define LEDG2			PORTD7

#define POS1_P1			PORTB2
#define POS1_P2			PORTB1
#define POS1_P3			PORTB0

#define POS2_P1			PORTD5
#define POS2_P2			PORTD3
#define POS2_P3			PORTD2


#define LIDSW			PINF5
#define LED_CTRL1		PORTC7
#define LED_CTRL2		PORTD1
#define LED_CTRL3		PORTD0

#define POS1_B1			0
#define POS1_B2			1
#define POS1_B3			2
#define POS2_B1			3
#define POS2_B2			4
#define POS2_B3			5

#define SWITCHCOUNT		6
#define MAINSTART		0
#define MAINSTOP		3
#define SECONDSTART		3
#define SECONDSTOP		6
#define LEDCOUNT		3
#define USBSW			1
#define SWITCHDELAY		1

#define LED1Index		0
#define LED2Index		1
#define LED3Index		2

#define MODECOUNT		5


#define INT_DECIMAL_STRING_SIZE(int_type) ((8*sizeof(int_type)-1)*10/33+3)

//#define SET_CURRENT		(float)15.0f


/************************************************************************/
/*					Timer Setup                                         */
/************************************************************************/

//Version Definitions
#define SHORT_DEBUGGING		1
#define LONGER_DEBUGGING	2
#define REAL_DEBUGGING_V1	3
#define REAL_DEBUGGING_V2	4
#define TCOMPANY_V1			5
#define TCOMPANY_V2			6
#define TCOMPANY_V3			7
#define SET_V1				8
#define SET_V2				9
#define SET_V3				10

//Simple Conversion Helpers
#define MS_HOUR			(millis_t)3600000
#define MS_MIN			(millis_t)60000
#define MS_SEC			(millis_t)1000
#define MIN_TO_MS(min)	(millis_t)((min)*MS_MIN)
#define HOUR_TO_MS(hr)	(millis_t)((hr)*MS_HOUR)
#define SEC_TO_MS(sec)	(millis_t)((sec)*MS_SEC)

#define DEBOUNCE		(millis_t)10

//Must Define Board Version Here
#define VERSION TCOMPANY_V3

//Change version parameters here
#if VERSION == SHORT_DEBUGGING
	#define AUTO_TIME  SEC_TO_MS	((millis_t)5)
	#define WAIT_TIME  SEC_TO_MS	((millis_t)10)
	#define CYCLE_TIME SEC_TO_MS	((millis_t)30)
#elif VERSION ==LONGER_DEBUGGING
	#define AUTO_TIME  MIN_TO_MS	((millis_t)30)
	#define WAIT_TIME  MIN_TO_MS	((millis_t)10)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)1)
#elif VERSION==REAL_DEBUGGING_V1
	#define AUTO_TIME  HOUR_TO_MS	((millis_t)2)
	#define WAIT_TIME  MIN_TO_MS	((millis_t)10)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)19)
#elif VERSION==REAL_DEBUGGING_V2
	#define AUTO_TIME  HOUR_TO_MS	((millis_t)3)
	#define WAIT_TIME  MIN_TO_MS	((millis_t)10)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)19)
	
#elif VERSION==TCOMPANY_V1
	#define BLINK					1
	#define SET_CURRENT				(float)13.3f
	#define AUTO_TIME  HOUR_TO_MS	((millis_t)4)
	#define WAIT_TIME  SEC_TO_MS	((millis_t)30)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)24)
#elif VERSION==TCOMPANY_V2
	#define BLINK					2
	#define SET_CURRENT				(float)13.3f
	#define AUTO_TIME  HOUR_TO_MS	((millis_t)1)
	#define WAIT_TIME  SEC_TO_MS	((millis_t)30)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)6)
#elif VERSION==TCOMPANY_V3
	#define BLINK					3
	#define SET_CURRENT				(float)13.3f
	#define AUTO_TIME  HOUR_TO_MS	((millis_t)2)
	#define WAIT_TIME  SEC_TO_MS	((millis_t)30)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)12)
	
#elif VERSION==SET_V1
	#define BLINK					2
	#define AUTO_TIME  HOUR_TO_MS	((millis_t)4)
	#define WAIT_TIME  MIN_TO_MS	((millis_t)10)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)24)
#elif VERSION==SET_V2
	#define BLINK					3
	#define AUTO_TIME  HOUR_TO_MS	((millis_t)4)
	#define WAIT_TIME  MIN_TO_MS	((millis_t)10)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)24)
#elif VERSION==SET_V3
	#define BLINK					4
	#define AUTO_TIME  HOUR_TO_MS	((millis_t)6)
	#define WAIT_TIME  MIN_TO_MS	((millis_t)10)
	#define CYCLE_TIME HOUR_TO_MS	((millis_t)24)
#else
	#error "Board Version Not Set"
#endif


#endif /* TBOARD_H_ */