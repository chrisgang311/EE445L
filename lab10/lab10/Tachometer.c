/** Tachometer.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 9th 2016
 * Description: Measures period with input capture
 * Lab: 10
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "Timer.h"
 
/** Measure Period **
 * Hook up the period measurement to PB7
 * Input is captured, and the period is the difference between
 * two rising edges
 */
 
// GPIO Port for Tachometer
static void PortB_Init(void);

// Motor Period reading (24-bit 12.5ns resolution)
static uint32_t period = 0;

/** Tachometer_Init() **
 * Activate the Tachometer for Input Loop.
 * Outputs: none
 */
void Tachometer_Init(){
	PortB_Init();
	Timer0B_Init(0x00); // highest priority
	Timer0B_Start();
}

/** Tachometer_Read() **
 * Reads the latest period measurement with 0.1rps resolution
 * Outputs: latest period value
 */
uint32_t Tachometer_Read(){
	return period;
}

// update the period value
// requires two rising edges.
void Timer0B_Handler(){
	Timer0B_Acknowledge();
	static uint32_t last = TIMER_TBILR_M;
	
	// calculate period
	uint32_t sample, timestamp;
	timestamp = TIMER0_TBR_R;
	if(last < timestamp){
		last = last + 0x00FFFFFF;
	} sample = (last - timestamp);
	sample = sample & 0x00FFFFFF;
	
	// update cache
	last = timestamp; 

	
	// noise filter
//  static uint32_t old = 0;
//	if(((old * 7 / 10) <= sample) && (sample <= (old * 13 / 10))){
		period = sample;
//	} old = sample;
	
	// sample averaging
//	const uint32_t count = 3;
//	static uint32_t idx = 0;
//	static uint32_t samples[count];
//	if(idx < count){
//		samples[idx] = sample;
//		idx = idx + 1;
//	} else{
//		idx = 0; period = 0; // average period
//		for(int i = 0; i < count; i++){
//			period = period + (samples[i] / count);
//		}
//	}
	
}

/** PortB_Init() **
 * Initialize PortB for Motor Output
 * Speaker output on PB6
 * Outputs: none
 */
static void PortB_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02;    		 // turn on port B
	delay = SYSCTL_RCGCTIMER_R; 			 // allow time to finish activating
	GPIO_PORTB_AFSEL_R |= 0x80;        // enable alt funct on PB7
  GPIO_PORTB_PCTL_R &= ~0xF0000000;  // configure PB7 as T0CP1
  GPIO_PORTB_PCTL_R |= 0x70000000;  
  GPIO_PORTB_DIR_R &= ~0x80;       // make PB7 in
  GPIO_PORTB_DEN_R |= 0x80;        // enable digital I/O on PB7
	

}
