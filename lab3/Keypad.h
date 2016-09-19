/** Keypad.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: Keyboard routines for our Alarm Clock
 * 4 tactile switch keypad to read user input.
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 14th 2016
 *********************************************************************************/
  
/** hardware connections **
 * Keypad Tactile Switches
 * SWa-d connected on PE1-4
 * SWa-d reads left to right on the breadboard.
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
 * bits0-3 represent switches a-d
 */
uint8_t Keypad_Read(void);
