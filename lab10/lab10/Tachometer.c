/** Tachometer.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 9th 2016
 * Description: Drives the motor output with a PWM
 * PWM connected on PB6
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
	
	// f = (pulses) / (fixed time)
	period = last - TIMER0_TBR_R; 
	period = period & 0x00FFFFFF;
	last = TIMER0_TBR_R; 
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
  GPIO_PORTB_PCTL_R &= ~0xF000000;  // configure PB7 as T0CP1
  GPIO_PORTB_PCTL_R |= 0x70000000;  
  GPIO_PORTB_DIR_R &= ~0x80;       // make PB7 in
  GPIO_PORTB_DEN_R |= 0x80;        // enable digital I/O on PB7
}
