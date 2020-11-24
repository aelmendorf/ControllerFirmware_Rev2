/*
 * millis.h
 *
 * Created: 3/8/2019 01:35:09 PM
 *  Implemented: Andrew Elmendorf
 * Created by Arduino
 */ 


#ifndef MILLIS_H_
#define MILLIS_H_

/**
* Milliseconds data type \n
* Data type				- Max time span			- Memory used \n
* unsigned char			- 255 milliseconds		- 1 byte \n
* unsigned int			- 65.54 seconds			- 2 bytes \n
* unsigned long			- 49.71 days			- 4 bytes \n
* unsigned long long	- 584.9 million years	- 8 bytes
*/
typedef unsigned long millis_t;

#define MILLIS_TIMER0 0 /**< Use timer0. */
#define MILLIS_TIMER1 1 /**< Use timer1. */

#ifndef MILLIS_TIMER
#define MILLIS_TIMER MILLIS_TIMER1 /**< Which timer to use. */
#endif

#ifndef ARDUINO
/**
* Alias of millis_get().
*
*/
#define millis() millis_get()
#endif

#ifdef __cplusplus
extern "C" {
	#endif
	void millis_init(void);
	millis_t millis_get(void);
	void millis_resume(void);
	void millis_pause(void);
	void millis_reset(void);
	void millis_add(millis_t ms);
	void millis_subtract(millis_t ms);
	#ifdef __cplusplus
}
#endif



#endif /* MILLIS_H_ */