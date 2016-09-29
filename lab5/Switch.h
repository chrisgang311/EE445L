/** Keypad.h **
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
 
/** Keypad_Init() **
 * Activate the Keypad for user input processing.
 * Outputs: none
 */
void Keypad_Init(void);

/** Keypad_Read() **
 * Sample the keypad for user input
 * A Set bit indicates the Switch is pressed.
 * Outputs: 8 bit number. 
 * bits0-2 represent switches a-c
 */
uint8_t Keypad_Read(void);
