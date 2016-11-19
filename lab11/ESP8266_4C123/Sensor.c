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
 #include <stdbool.h>
 
#include "tm4c123gh6pm.h"

// sensor flag
static bool triggered;
static bool enabled;

// port init for Sensor
static void PortB_Init(void); 

/** Sensor_Init() **
 * Activate the Motion Sensor for listening
 * Outputs: none
 */
void Sensor_Init(void){
	PortB_Init();
	triggered = false;
	enabled = false;
}

/** Sensor_Reset() **
 * Resets the motion sensor flag.
 * Triggered status is cleared.
 */
void Sensor_Reset(void){
	triggered = false;
}

/** Sensor_Triggered() **
 * Check if the motion sensor was triggered
 * returns true if sensor was triggered
 */
bool Sensor_Triggered(void){
	return triggered && enabled;
}

/** Sensor_Enable() **
 * Enable the motion sensor edge trigger
 */
void Sensor_Enable(void){
	enabled = true;
	triggered = false;
	GPIO_PORTB_IM_R |= 0x80; // arm interrupt
}

/** Sensor_Disable() **
 * Disable the motion sensor edge trigger
 */
void Sensor_Disable(void){
	enabled = false;
	triggered = false;
	GPIO_PORTB_IM_R &= ~0x80; // disarm interrupt
}

// sensor edge interrupt
void GPIOPortB_Handler(void){
  GPIO_PORTB_ICR_R = 0x80; // acknowledge sensor interrupt
	if(enabled){
		triggered = true;
	}
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
	GPIO_PORTB_AFSEL_R &= ~0x80;        // disable alt funtion on PB7
  GPIO_PORTB_PCTL_R &= ~0xF0000000;
  GPIO_PORTB_AMSEL_R &= ~0x80;       // disable analog functionality on PB7
  GPIO_PORTB_DEN_R |= 0x80;          // enable digital I/O on PB7
	GPIO_PORTB_DIR_R &= ~0x80;					// Set PB7 as input
	
	// edge trigger
	GPIO_PORTB_IS_R &= ~0x80;					
	GPIO_PORTB_IBE_R &= ~0x80;
	GPIO_PORTB_IEV_R |= 0x80; 					// rising edge.
	GPIO_PORTB_ICR_R |= 0x80;						// acknowledge flag clear
	//GPIO_PORTB_IM_R |= 0x80;						// arm interrupt

  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)|0x00004000; // priority 2
	NVIC_EN0_R = 1 << 1;      // enable interrupt 1 in NVIC (port b handler)
}
