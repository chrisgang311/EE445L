/** Timer.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 9th 2016
 * Description: Timer methods for various interrupt purposes
 * utitilizes various timer interrupts
 * Lab: 1
 * TA: Dylan Zika
 * Date: September 9th 2016
 *********************************************************************************/
 
 #define TIMER_100Hz 799999
 #define TIMER_1000Hz 79999
 #define TIMER_MAXHz 79999  // 53s at 80MHz clock
 
 
// timer initializations

/** Timer0A_Init **
 * Activate TIMER0A to countdown for period seconds
 * Initializes Timer0A for period interrupts
 * Inputs:  period (reload value for the timer)
 * Outputs: none
 */
void Timer0A_Init(uint32_t period);

/** Timer1A_Init **
 * Activate TIMER1A to countdown for period seconds (No Interrupts)
 * Inputs:  period (reload value for the timer)
 * Outputs: none
 */
void Timer1A_Init(uint32_t period); 

/** Timer0A_Stop() **
 * Stop the Clock (TIMER 0A)
 */
void Timer0A_Stop(void);

/** Timer0A_Arm() **
 * Enable interrupts from Timer0A.
 */
void Timer0A_Arm(void);

/** Timer0A_Disarm() **
 * Disable interrupts from Timer0A.
 */
void Timer0A_Disarm(void);
