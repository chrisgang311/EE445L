/** Timer.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 9th 2016
 * Description: Timer methods for various interrupt purposes
 * utitilizes various timer interrupts
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
 #include <stdint.h>
 #include "tm4c123gh6pm.h"
 
// process managment
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

/** Timer0A_Init() **
 * Activate TIMER0A to countdown for period seconds
 * Initializes Timer0A for period interrupts
 * Inputs:  period (reload value for the timer)
            priority interrupt importance 0-7. 0 is highest priority
 * Outputs: none
 */
void Timer0A_Init(uint32_t period, uint32_t priority){
  volatile uint32_t delay;
  DisableInterrupts();
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
	TIMER0_TAPR_R = 0;           		 // bus clock resolution
                                   
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD; // configure for periodic mode
  TIMER0_TAILR_R = period;         // start value for periodic interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  
	// **** interrupt initialization ****
	priority = (priority & 0x07) << 29; // mask priority (nvic bits 29-31)
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF); // clear priority
	NVIC_PRI4_R = (NVIC_PRI4_R | priority); // set interrupt priority bits
  NVIC_EN0_R = 1 << 19;              // enable interrupt 19 in NVIC
	EnableInterrupts();
}

/** Timer1A_Init() **
 * Activate TIMER1A with periodic interrupts
 * Inputs:  period (reload value for the timer)
            priority interrupt importance 0-7. 0 is highest priority
 * Outputs: none
 */
void Timer1A_Init(uint32_t period, uint32_t priority){
  volatile uint32_t delay;
	DisableInterrupts();
  SYSCTL_RCGCTIMER_R |= 0x02;   // activate TIMER1
  delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
  TIMER1_CTL_R = ~TIMER_CTL_TAEN;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0;                 // configure for 32-bit timer mode
	TIMER1_TAPR_R = 0;               // bus clock resolution
	
  TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;   // 3) configure for periodic mode, down-count 
  TIMER1_TAILR_R = period;  // 4) reload value
	TIMER1_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// clear timer1A timeout flag
	
	// **** interrupt initialization ****
	priority = (priority & 0x07) << 13; // mask priority (nvic bits 15-13)
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF); // clear priority
	NVIC_PRI5_R = (NVIC_PRI5_R | priority); // set interrupt priority bits
  NVIC_EN0_R = 1 << 21;              // enable interrupt 21 in NVIC
	EnableInterrupts();
}

/** Timer2A_Init() **
 * Activate TIMER2A to countdown for period seconds
 * Initializes Timer2A for period interrupts
 * Inputs:  period (reload value for the timer)
            priority interrupt importance 0-7. 0 is highest priority
 * Outputs: none
 */
void Timer2A_Init(uint32_t period, uint32_t priority){
	volatile uint32_t delay;
	DisableInterrupts();
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2
	delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
	TIMER2_CTL_R = ~TIMER_CTL_TAEN;    // 1) disable TIMER2A during setup
	TIMER2_CFG_R = 0;    // 2) configure for 32-bit mode
	TIMER2_TAPR_R = 0;            // 5) bus clock resolution
	
	TIMER2_TAMR_R = TIMER_TAMR_TAMR_PERIOD;   // 3) configure for periodic mode, default down-count settings
	TIMER2_TAILR_R = period;    	// 4) reload value
	TIMER2_IMR_R = TIMER_IMR_TATOIM;// arm timeout interrupt
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;    // 6) clear TIMER2A timeout flag

	// Timer 2 interupts
	priority = (priority & 0x07) << 29; // mask priority (nvic bits 15-13)
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF); // clear priority
	NVIC_PRI5_R = (NVIC_PRI5_R | priority); // set interrupt priority bits
  NVIC_EN0_R = 1 << 23;              // enable interrupt 23 in NVIC
	EnableInterrupts();
}	

/** Timer3A_Init() **
 * Activate Timer3A to countdown for period seconds
 * Initializes Timer3A for period interrupts
 * Inputs:  period (reload value for the timer)
            priority interrupt importance 0-7. 0 is highest priority
 * Outputs: none
 */
void Timer3A_Init(uint32_t period, uint32_t priority){
	volatile uint32_t delay;
	DisableInterrupts();
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate Timer3
	delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
	TIMER3_CTL_R = ~TIMER_CTL_TAEN;    // 1) disable TIMER3A during setup
	TIMER3_CFG_R = 0;    // 2) configure for 32-bit mode
	TIMER3_TAPR_R = 0;            // 5) bus clock resolution
	
	TIMER3_TAMR_R = TIMER_TAMR_TAMR_PERIOD;   // 3) configure for periodic mode, default down-count settings
	TIMER3_TAILR_R = period;    	// 4) reload value
	TIMER3_IMR_R = TIMER_IMR_TATOIM;// arm timeout interrupt
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;    // 6) clear TIMER3A timeout flag

	// Timer 3 interupts
	priority = (priority & 0x07) << 29; // mask priority (nvic bits 15-13)
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF); // clear priority
	NVIC_PRI8_R = (NVIC_PRI8_R | priority); // set interrupt priority bits
  NVIC_EN1_R = 1 << 3;              // enable interrupt 35 in NVIC
	EnableInterrupts();
}	

/******************* Timer0A Methods ****************************/

/** Timer0A_Start() **
 * Restart the Clock (TIMER 0A)
 */
void Timer0A_Start(){
	TIMER0_CTL_R |= TIMER_CTL_TAEN;
}

/** Timer0A_Stop() **
 * Stop the Clock (TIMER 0A)
 */
void Timer0A_Stop(){
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
}

/** Timer0A_Arm() **
 * Enable interrupts from Timer0A.
 */
void Timer0A_Arm(){
	NVIC_EN0_R = 1 << 19;
}

/** Timer0A_Disarm() **
 * Disable interrupts from Timer0A.
 */
void Timer0A_Disarm(){
	NVIC_DIS0_R = 1 << 19;
}

/** Timer0A_Period() **
 * Acknowledge a Timer0A interrupt
 */
void Timer0A_Acknowledge(){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT; 
}

/** Timer0A_Period() **
 * Reset the period on Timer0A
 */
void Timer0A_Period(uint32_t period){
	TIMER0_TAILR_R = period; 
}

/******************* Timer1A Methods ****************************/

/** Timer1A_Start() **
 * Restart the Clock (TIMER 1A)
 */
void Timer1A_Start(){
	TIMER1_CTL_R |= TIMER_CTL_TAEN;
}

/** Timer1A_Stop() **
 * Stop the Clock (TIMER 1A)
 */
void Timer1A_Stop(){
	TIMER1_CTL_R &= ~TIMER_CTL_TAEN;
}

/** Timer1A_Arm() **
 * Enable interrupts from Timer1A.
 */
void Timer1A_Arm(){
	NVIC_EN0_R = 1 << 21;
}

/** Timer1A_Disarm() **
 * Disable interrupts from Timer1A.
 */
void Timer1A_Disarm(){
	NVIC_DIS0_R = 1 << 21;
}

/** Timer1A_Period() **
 * Acknowledge a Timer1A interrupt
 */
void Timer1A_Acknowledge(){
	TIMER1_ICR_R = TIMER_ICR_TATOCINT; 
}

/** Timer1A_Period() **
 * Reset the period on Timer1A
 */
void Timer1A_Period(uint32_t period){
	TIMER1_TAILR_R = period; 
}

/******************* Timer2A Methods ****************************/

/** Timer2A_Start() **
 * Restart the Clock (TIMER 2A)
 */
void Timer2A_Start(){
	TIMER2_CTL_R |= TIMER_CTL_TAEN;
}

/** Timer2A_Stop() **
 * Stop the Clock (TIMER 2A)
 */
void Timer2A_Stop(){
	TIMER2_CTL_R &= ~TIMER_CTL_TAEN;
}

/** Timer2A_Arm() **
 * Enable interrupts from Timer2A.
 */
void Timer2A_Arm(){
	NVIC_EN0_R = 1 << 23;
}

/** Timer2A_Disarm() **
 * Disable interrupts from Timer2A.
 */
void Timer2A_Disarm(){
	NVIC_DIS0_R = 1 << 23;
}

/** Timer2A_Acknowledge() **
 * Acknowledge a Timer2A interrupt
 */
void Timer2A_Acknowledge(){
	TIMER2_ICR_R = TIMER_ICR_TATOCINT; 
}

/** Timer2A_Period() **
 * Reset the period on Timer2A
 */
void Timer2A_Period(uint32_t period){
	TIMER2_TAILR_R = period; 
}

/******************* Timer3A Methods ****************************/

/** Timer3A_Start() **
 * Restart the Clock (TIMER 2A)
 */
void Timer3A_Start(){
	TIMER3_CTL_R |= TIMER_CTL_TAEN;
}

/** Timer3A_Stop() **
 * Stop the Clock (TIMER 2A)
 */
void Timer3A_Stop(){
	TIMER3_CTL_R &= ~TIMER_CTL_TAEN;
}

/** Timer3A_Arm() **
 * Enable interrupts from Timer3A.
 */
void Timer3A_Arm(){
	NVIC_EN1_R = 1 << 3;
}

/** Timer3A_Disarm() **
 * Disable interrupts from Timer3A.
 */
void Timer3A_Disarm(){
	NVIC_DIS1_R = 1 << 3;
}

/** Timer3A_Acknowledge() **
 * Acknowledge a Timer3A interrupt
 */
void Timer3A_Acknowledge(){
	TIMER3_ICR_R = TIMER_ICR_TATOCINT; 
}

/** Timer3A_Period() **
 * Reset the period on Timer3A
 */
void Timer3A_Period(uint32_t period){
	TIMER3_TAILR_R = period; 
}

/** old delay function **/
//void Timer1A_Wait(uint32_t delay){
//  uint32_t elapsedTime;
//  uint32_t startTime = TIMER1_TAR_R;
//  while(elapsedTime <= delay){
//    elapsedTime = startTime-TIMER1_TAR_R;
//  }
//}

