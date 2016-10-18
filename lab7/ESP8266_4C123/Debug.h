/** Debug.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 27th 2016
 * Description: Vairous methods for debugging our embedded systems
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
 #include <stdbool.h>
 
 /** Debug_Init() **
 * Initializes our debugging tools
 */
void Debug_Init(void);

/** Debug_Wait10Ms() **
 * Pause execution for 10ms
 * busy wait on systick
 */
void Debug_Wait10ms(void);

/** Debug_Switch1() **
 * Return the value of board switch 1
 */
bool Debug_Switch1(void);

/** Debug_Switch2() **
 * Return the value of board switch 2
 */
bool Debug_Switch2(void);
