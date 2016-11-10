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

/** Keypad_Init() **
 * Activate the Keypad for user input processing.
 * Outputs: none
 */
void Keypad_Init(void);

/** Keypad_Read() **
 * Sample the keypad for user input
 * A Set bit indicates the Switch is pressed.
 * Outputs: 8 bit number. 
 * bits0-1 represent switches 0 and 1
 */
uint8_t Keypad_Read(void);
