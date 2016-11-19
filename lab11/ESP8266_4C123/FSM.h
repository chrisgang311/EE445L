/** FSM.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 18th 2016
 * Description: FSM Logic for main home alarm system
 * Lab: 8
 * TA: Dylan Zika
 * Date: October 31st 2016
 *********************************************************************************/
 
/** UpdateFSM() **
 * Process the user's input from the keypad.
 * Implements a mealy FSM.
 *
 * System States
 * 0x00 main settings screen
 * 0x01 set passcode screen
 * 0x02 enter passcode screen
 * 0x03 alarm idle
 */
uint32_t UpdateFSM(uint32_t state, uint32_t input);
