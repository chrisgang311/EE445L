/** Keypad.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: Keyboard routines for our Alarm System
 * 16-button keypad is interfaced on Port C and D
 * Lab: 7
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
/** hardware connections **
 * Keypad Tactile Switches
 * Col 1-3 : PD0-3 inputs
 * Row 1-3 : PC4-7 outputs
 */
 
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Keypad.h"

// port C and D init for Keypad hardware
static void PortC_Init(void);
static void PortD_Init(void);
 
/** Keypad_Init() **
 * Activate the Keypad for user input processing.
 * Outputs: none
 */
void Keypad_Init(){
	PortC_Init();
	PortD_Init();
}

/** Keypad_Read() **
 * Sample the keypad for user input
 * A Set bit indicates the Switch is pressed.
 * Outputs: 16 bit number. 
 * Read from the bit codes as follows
 */
uint16_t Keypad_Read(){
	// check one row at a time.
	uint16_t shift = 0, input = 0x0000;
	for(uint8_t row = 0x10; row != 0x00; row <<= 1){
		// source output on single row
		GPIO_PORTC_DATA_R &= ~0xF0;
		GPIO_PORTC_DATA_R |= row; 
		
		// check each column
		input |= (GPIO_PORTD_DATA_R&0x0F) << shift;
		shift += 4; // 4 checks at a time.
	} return input; // keypad encoded values
}

/** KeyConvert() **
 * Returns the character of the key value
 * input: 16 bit keycode
 * output: char value
 */
char KeyConvert(uint16_t key){
	switch(key){
		case KEY_0:
			return '0';
		case KEY_1:
			return '1';
		case KEY_2:
			return '2';
		case KEY_3:
			return '3';
		case KEY_4:
			return '4';
		case KEY_5:
			return '5';
		case KEY_6:
			return '6';
		case KEY_7:
			return '7';
		case KEY_8:
			return '8';
		case KEY_9:
			return '9';
		case KEY_A:
			return 'A';
		case KEY_B:
			return 'B';
		case KEY_C:
			return 'C';
		case KEY_D:
			return 'D';
		case KEY_POUND:
			return '#';
		case KEY_ASTERISK:
			return '*';
		default:
			return ' ';
	}
}

/** PortD_Init() **
 * Initialize PortD for Column Input Processing
 * Four Column inputs: Col 1-4 on PD0-3
 * Outputs: none
 */
static void PortD_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x08;     // 1) turn on port D
	delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
	GPIO_PORTD_AMSEL_R &= ~0x0F; // 2) disable analog on PD0-3
	GPIO_PORTD_AFSEL_R &= ~0x0F; // 3) disable alternative functionality on PD0-3
	GPIO_PORTD_PCTL_R &= ~0x0000FFFF;  // 4) choose GPIO functionality
	GPIO_PORTD_DIR_R &= ~0x0F;   // 5) set PD0-3 to read inputs
	GPIO_PORTD_DEN_R |= 0x0F;    // 6) inputs are digital
}

/** PortC_Init() **
 * Initialize PortC for Row Output Processing
 * Four Column inputs: Row 1-4 on PC4-7
 * Outputs: none
 */
static void PortC_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x04;     // 1) turn on port C
	delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
	GPIO_PORTC_AMSEL_R &= ~0xF0; // 2) disable analog on PC4-7
	GPIO_PORTC_AFSEL_R &= ~0xF0; // 3) disable alternative functionality on PC4-7
	GPIO_PORTC_PCTL_R &= ~0xFFFF0000;  // 4) choose GPIO functionality
	GPIO_PORTC_DIR_R |= 0xF0;   // 5) set PC4-7 to drive outputs
	GPIO_PORTC_DEN_R |= 0xF0;    // 6) inputs are digital
}
