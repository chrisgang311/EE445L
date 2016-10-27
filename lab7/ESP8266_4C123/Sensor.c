/** Sensor.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: October 25th 2016
 * Description: Motion Sensor interface implementation.
 * Recieve and Handle an alarm notification from the motion sensor.
 * Lab: 7
 * TA: Dylan Zika
 * Date: October 25th 2016
 *********************************************************************************/
 
/** hardware connections **
 * SE-10 Motion sensor
 * Alarm edge input on PB7
 */
 
 #include <stdint.h>
#include "tm4c123gh6pm.h"

// port init for Sensor
static void PortB_Init(void); 

/** Sensor_Init() **
 * Activate the Motion Sensor for listening
 * Outputs: none
 */
void Sensor_Init(void){
	PortB_Init();
}

/** PortB_Init() **
 * Initialize PortB for Sensor Input
 * Set up an edge-triggered interrupt
 * Speaker output on PB7
 * Outputs: none
 */
static void PortB_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02;    		 // turn on port B
	delay = SYSCTL_RCGCTIMER_R; 			 // allow time to finish activating
	GPIO_PORTB_AFSEL_R |= 0x80;        // disable alt funtion on PB7
  GPIO_PORTB_PCTL_R &= ~0xF0000000;
  GPIO_PORTB_AMSEL_R &= ~0x80;       // disable analog functionality on PB7
  GPIO_PORTB_DEN_R |= 0x80;          // enable digital I/O on PB7
	GPIO_PORTB_DIR_R &= ~0x80;					// Set PB7 as input
}
