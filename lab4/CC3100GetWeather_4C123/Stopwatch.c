/** Stopwatch.c **
 * Authors: Ronald Macmaster and Parth Adhia 
 * Created: September 25th 2016
 * Description: Stopwatch driver for measuring running time
 * Lab: 4
 * TA: Dylan Zika
 * Date: September 25th 2016
 *********************************************************************************/
 
 #include <stdint.h>
 #include "tm4c123gh6pm.h"
 
 #define STOPWATCH_RELOAD 0x00FFFFFF
 
/** Stopwatch_Start() **
 * Begin ticking the stopwatch
 * Input: none
 * Output: start time
 */
uint32_t Stopwatch_Start(void){
	uint32_t time = 0;
	NVIC_ST_CTRL_R = 0x0000; // disable countdown
	NVIC_ST_RELOAD_R = STOPWATCH_RELOAD; // Maximum value preferably
	NVIC_ST_CURRENT_R = 0; // clear systick reg.
	NVIC_ST_CTRL_R = 0x0001; // enable countdown
	return time;
}

/** Stopwatch_Start() **
 * Stop ticking the stopwatch
 * Input: none
 * Output: finish time
 */
uint32_t Stopwatch_Stop(void){
	uint32_t time = NVIC_ST_CURRENT_R;
	NVIC_ST_CTRL_R = 0x0000; // stop countdown
	time = STOPWATCH_RELOAD - time; // stopwatch ticks up
	return time;
}
