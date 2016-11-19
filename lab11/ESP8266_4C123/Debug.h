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

// LED Hardware Connections
// RED LED ON PB2, YELLOW on PB3

//------------LED_RedOn------------
// Turn on red LED
// Input: none
// Output: none
void LED_RedOn(void);
  
//------------LED_RedOff------------
// Turn off red LED
// Input: none
// Output: none
void LED_RedOff(void);

//------------LED_RedToggle------------
// Toggle red LED
// Input: none
// Output: none
void LED_RedToggle(void);
  
//------------LED_YellowToggle------------
// Toggle yellow LED
// Input: none
// Output: none
void LED_YellowToggle(void);

//------------LED_YellowOn------------
// Turn on yellow LED
// Input: none
// Output: none
void LED_YellowOn(void);

//------------LED_YellowOff------------
// Turn off yellow LED
// Input: none
// Output: none
void LED_YellowOff(void);

