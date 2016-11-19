/** Timer.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 9th 2016
 * Description: Timer methods for various interrupt purposes
 * utitilizes various timer interrupts
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
 // 80MHZ Clock frequencies
 #define TIMER_1Hz 79999999
 #define TIMER_100Hz 799999
 #define TIMER_1000Hz 79999
 #define TIMER_MAXHz 79999999  // 53s at 80MHz clock
 #define TIMER_DELAY1S_1000Hz 519999999
 
// timer initializations

/** Timer0A_Init() **
 * Activate TIMER0A to countdown for period seconds
 * Initializes Timer0A for period interrupts
 * Inputs:  period (reload value for the timer)
            priority interrupt importance 0-7. 0 is highest priority
 * Outputs: none
 */
void Timer0A_Init(uint32_t period, uint32_t priority);

/** Timer1A_Init() **
 * Activate TIMER1A with periodic interrupts
 * Inputs:  period (reload value for the timer)
            priority interrupt importance 0-7. 0 is highest priority
 * Outputs: none
 */
void Timer1A_Init(uint32_t period, uint32_t priority);

/** Timer2A_Init() **
 * Activate TIMER2A to countdown for period seconds
 * Initializes Timer2A for period interrupts
 * Inputs:  period (reload value for the timer)
            priority interrupt importance 0-7. 0 is highest priority
 * Outputs: none
 */
void Timer2A_Init(uint32_t period, uint32_t priority);

/** Timer3A_Init() **
 * Activate TIMER3A to countdown for period seconds
 * Initializes Timer3A for period interrupts
 * Inputs:  period (reload value for the timer)
            priority interrupt importance 0-7. 0 is highest priority
 * Outputs: none
 */
void Timer3A_Init(uint32_t period, uint32_t priority);


/******************* Timer0A Methods ****************************/

/** Timer0A_Start() **
 * Restart the Clock (TIMER 0A)
 */
void Timer0A_Start(void);

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

/** Timer0A_Period() **
 * Acknowledge a Timer0A interrupt
 */
void Timer0A_Acknowledge(void);

/** Timer0A_Period() **
 * Reset the period on Timer0A
 */
void Timer0A_Period(uint32_t period);

/******************* Timer1A Methods ****************************/

/** Timer1A_Start() **
 * Restart the Clock (TIMER 1A)
 */
void Timer1A_Start(void);

/** Timer1A_Stop() **
 * Stop the Clock (TIMER 1A)
 */
void Timer1A_Stop(void);

/** Timer1A_Arm() **
 * Enable interrupts from Timer1A.
 */
void Timer1A_Arm(void);

/** Timer1A_Disarm() **
 * Disable interrupts from Timer1A.
 */
void Timer1A_Disarm(void);

/** Timer1A_Period() **
 * Acknowledge a Timer1A interrupt
 */
void Timer1A_Acknowledge(void);

/** Timer1A_Period() **
 * Reset the period on Timer1A
 */
void Timer1A_Period(uint32_t period);

/******************* Timer2A Methods ****************************/

/** Timer2A_Start() **
 * Restart the Clock (TIMER 2A)
 */
void Timer2A_Start(void);

/** Timer2A_Stop() **
 * Stop the Clock (TIMER 2A)
 */
void Timer2A_Stop(void);

/** Timer2A_Arm() **
 * Enable interrupts from Timer2A.
 */
void Timer2A_Arm(void);

/** Timer2A_Disarm() **
 * Disable interrupts from Timer2A.
 */
void Timer2A_Disarm(void);

/** Timer2A_Period() **
 * Acknowledge a Timer2A interrupt
 */
void Timer2A_Acknowledge(void);


/** Timer2A_Period() **
 * Reset the period on Timer2A
 */
void Timer2A_Period(uint32_t period);

/******************* Timer3A Methods ****************************/

/** Timer3A_Start() **
 * Restart the Clock (TIMER 3A)
 */
void Timer3A_Start(void);

/** Timer3A_Stop() **
 * Stop the Clock (TIMER 3A)
 */
void Timer3A_Stop(void);

/** Timer3A_Arm() **
 * Enable interrupts from Timer3A.
 */
void Timer3A_Arm(void);

/** Timer3A_Disarm() **
 * Disable interrupts from Timer3A.
 */
void Timer3A_Disarm(void);

/** Timer3A_Period() **
 * Acknowledge a Timer3A interrupt
 */
void Timer3A_Acknowledge(void);


/** Timer3A_Period() **
 * Reset the period on Timer3A
 */
void Timer3A_Period(uint32_t period);
