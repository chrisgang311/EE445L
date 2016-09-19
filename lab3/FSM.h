/** FSM.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 18th 2016
 * Description: FSM Logic for main clock program
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 18th 2016
 *********************************************************************************/
 
/** UpdateClock() **
 * Process the user's input from the keypad.
 * Implements a mealy FSM.
 *
 * Alarm Clock States
 * 0x00 main screen
 * 0x01 configure time
 * 0x02 configure alarm
 * 0x03 play/mute alarm
 */
uint32_t UpdateClock(uint32_t state, uint32_t input);

/** MainScreen() **
 * Process the user's input from the keypad.
 * Main screen displays a digital or graphical clock.
 * Also displays some help controls.
 */
uint32_t MainScreen(uint32_t input);
 
/** ConfigureTime() **
 * Process the user's input from the keypad.
 * Allows the user to set the current clock time.
 * Also displays some help controls.
 */
uint32_t ConfigureTime(uint32_t input);
 
 /** ConfigureAlarm() **
 * Process the user's input from the keypad.
 * Allows the user to set the future alarm time.
 * Also displays some help controls.
 */
uint32_t ConfigureAlarm(uint32_t input);

/** ConfigureTime() **
 * Process the user's input from the keypad.
 * Play or mute the speaker.
 * Display the on-board animation.
 * Also displays some help controls.
 */
uint32_t ControlAlarm(uint32_t input);
