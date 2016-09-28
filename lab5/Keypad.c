/** Keypad.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: Keyboard routines for our Alarm Clock
 * 3 tactile switch keypad to read user input.
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
/** hardware connections **
 * Keypad Tactile Switches
 * SWa-c connected on PE1-3
 * SWa-c reads left to right on the breadboard.
 */
 
#include <stdint.h>
#include "tm4c123gh6pm.h"

// port E init for Keypad hardware
static void PortE_Init(void);
 
/** Keypad_Init() **
 * Activate the Keypad for user input processing.
 * Outputs: none
 */
void Keypad_Init(){
	PortE_Init();
}

/** Keypad_Read() **
 * Sample the keypad for user input
 * A Set bit indicates the Switch is pressed.
 * Outputs: 8 bit number. 
 * bits0-2 represent switches a-c
 */
uint8_t Keypad_Read(){
	// mask logic switch input on bits 0-2
	return (GPIO_PORTE_DATA_R >> 1)&0x07; 
}

/** PortE_Init() **
 * Initialize PortE for Input Processing
 * Four Switch inputs: SWa-c on PE1-3 
 * Outputs: none
 */
static void PortE_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x10;     // 1) turn on port E
	delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
	GPIO_PORTE_AMSEL_R &= ~0x0E; // 2) disable analog on PE1-3
	GPIO_PORTE_AFSEL_R &= ~0x0E; // 3) disable alternative functionality on PE1-3
	GPIO_PORTE_PCTL_R &= ~0x0E;  // 4) choose GPIO functionality
	GPIO_PORTE_DIR_R &= ~0x0E;   // 5) set PE1-3 to read inputs
	GPIO_PORTE_DEN_R |= 0x0E;    // 6) inputs are digital
}
