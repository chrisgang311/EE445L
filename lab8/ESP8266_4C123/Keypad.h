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
 
 
/** Keypad_Init() **
 * Activate the Keypad for user input processing.
 * Outputs: none
 */
void Keypad_Init(void);

/** Keypad_Read() **
 * Sample the keypad for user input
 * A Set bit indicates the Switch is pressed.
 * Outputs: 16 bit number. 
 * Read from the bit codes as follows
 */
uint16_t Keypad_Read(void);

/** KeyConvert() **
 * Returns the character of the key value
 * input: 16 bit keycode
 * output: char value
 */
char KeyConvert(uint16_t key);

#ifndef KEY_CODES
#define KEY_CODES
#define KEY_0 0x2000
#define KEY_1 0x0001
#define KEY_2 0x0002
#define KEY_3 0x0004
#define KEY_4 0x0010
#define KEY_5 0x0020
#define KEY_6 0x0040
#define KEY_7 0x0100
#define KEY_8 0x0200
#define KEY_9 0x0400
#define KEY_A 0x0008
#define KEY_B 0x0080
#define KEY_C 0x0800
#define KEY_D 0x8000
#define KEY_POUND 0x4000
#define KEY_ASTERISK 0x1000
#endif // KEY_CODES
