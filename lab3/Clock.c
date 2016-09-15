/** Clock.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: Alarm implementation of abstract the Alarm Clock interface
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 14th 2016
 *********************************************************************************/

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer.h"
#include "PLL.h"

// alarm and clock time values
// 32-bit packed time value. top 16bits are hour, bottom 16 are minute.
// alt: 24 bit-packed time value with 8 bit sections: (0xhhmmss)
static uint32_t ClockTime, AlarmTime;

/** Clock_Init() **
 * Initialize the alarm clock (Plug in the alarm clock)
 * Must be called first
 */
void Clock_Init(void){
	ClockTime = AlarmTime = 0;
	PLL_Init(Bus80MHz);
	Timer0A_Init(TIMER_1Hz/2);
}

/** Clock_Read() **
 * Read the clock time.
 */
uint32_t Clock_Read(void){
	return ClockTime;
}

/** Alarm_Set() ** 
 * Set the alarm
 */
void Alarm_Set(uint32_t time){
	AlarmTime = time;
}

/** Alarm_Read() ** 
 * Read the alarm value.
 */
uint32_t Alarm_Read(void){
	return AlarmTime;
}



