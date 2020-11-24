/*
 * TBoard.h
 *
 * Created: 2/18/2019 10:15:46 AM
 *  Author: 600076
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
#include <CurrentDriver.h>
#include <EEPROM.h>
#include <millis.h>
#include "m_general.h"
#include "m_usb.h"


#define TRUE			(uint8_t)1
#define FALSE			(uint8_t)0

#define HIGH			(uint8_t)1
#define LOW				(uint8_t)0
//Ports
#define RED				PORTB4
#define BLUE			PORTB6
#define GREEN			PORTB5
#define LID_SWITCH		PINF5
#define CURRENT_DRIVER	PORTC7
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