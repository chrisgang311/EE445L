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
 * Outputs: none
 */
void Timer0A_Init(uint32_t period){
  volatile uint32_t delay;
  DisableInterrupts();
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
                                   
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD; // configure for periodic mode
  TIMER0_TAILR_R = period;         // start value for periodic interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
	EnableInterrupts();
}

/** Timer1A_Init() **
 * Activate TIMER1A to countdown for period seconds (No Interrupts)
 * Inputs:  period (reload value for the timer)
 * Outputs: none
 */
void Timer1A_Init(uint32_t period){
  volatile uint32_t delay;
	DisableInterrupts();
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
  TIMER1_CTL_R = ~TIMER_CTL_TAEN;    // 1) disable TIMER1A during setup
 	TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, down-count 
  TIMER1_TAILR_R = 0xFFFFFFFF;  // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_CTL_R = TIMER_CTL_TAEN;    // 10) enable TIMER1A
	EnableInterrupts();
}

/** Timer2A_Init() **
 * Activate TIMER2A to countdown for period seconds
 * Initializes Timer2A for period interrupts
 * Inputs:  period (reload value for the timer)
 * Outputs: none
 */
void Timer2A_Init(uint32_t period){
	DisableInterrupts();
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2
	TIMER2_CTL_R = 0x00000000;    // 1) disable TIMER2A during setup
	TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
	TIMER2_TAILR_R = period;    	// 4) reload value
	TIMER2_TAPR_R = 0;            // 5) bus clock resolution
	TIMER2_ICR_R = 0x00000001;    // 6) clear TIMER2A timeout flag

	// Timer 2 interupts
	TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0xE0000000; 
  NVIC_EN0_R |= 1 << 23;           // 9) enable IRQ 19 in NVIC
	TIMER2_CTL_R = 0x00000001;    // 10) Turn on Timer 2
	EnableInterrupts();
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

/** Timer2A_Arm() **
 * Enable interrupts from Timer0A.
 */
void Timer2A_Arm(){
	NVIC_EN0_R = 1 << 23;
}

/** Timer0A_Disarm() **
 * Disable interrupts from Timer0A.
 */
void Timer2A_Disarm(){
	NVIC_DIS0_R = 1 << 23;
}

void Timer1A_Wait(uint32_t delay){
  uint32_t elapsedTime;
  uint32_t startTime = TIMER1_TAR_R;
  while(elapsedTime <= delay){
    elapsedTime = startTime-TIMER1_TAR_R;
  }
}
		
