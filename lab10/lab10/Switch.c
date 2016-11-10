/** Switch.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: Switch routines for our user input
 * 2 tactile switch keypad to read user input.
 * Lab: 10
 * TA: Dylan Zika
 * Date: November 9th 2016
 *********************************************************************************/
 
/** hardware connections **
 * Keypad Tactile Switches
 * SW0 & SW1 connected on PE2-3
 * Switches read left to right on breadboard
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
 * bits0-1 represent switches 0 and 1
 */
uint8_t Keypad_Read(){
	// mask logic switch input on bits 0-2
	return (GPIO_PORTE_DATA_R >> 2)&0x03; 
}

/** PortE_Init() **
 * Initialize PortE for Input Processing
 * Four Switch inputs: SW0 and SW1 on PE2-3 
 * Outputs: none
 */
static void PortE_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x10;     // 1) turn on port E
	delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
	GPIO_PORTE_AMSEL_R &= ~0x0C; // 2) disable analog on PE1-3
	GPIO_PORTE_AFSEL_R &= ~0x0C; // 3) disable alternative functionality on PE1-3
	GPIO_PORTE_PCTL_R &= ~0x0C;  // 4) choose GPIO functionality
	GPIO_PORTE_DIR_R &= ~0x0C;   // 5) set PE1-3 to read inputs
	GPIO_PORTE_DEN_R |= 0x0C;    // 6) inputs are digital
}
