/** Debug.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 27th 2016
 * Description: Vairous methods for debugging our embedded systems
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
 #include <stdint.h>
 #include <stdbool.h>
 #include "tm4c123gh6pm.h"

// GPIO port F
#define PF0       (*((volatile uint32_t *)0x40025004))
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF4       (*((volatile uint32_t *)0x40025040))
static void PortF_Init(void);

// SysTick countdown
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value
void SysTick_Init(void);
void SysTick_Wait(uint32_t delay);
void SysTick_Wait10ms(void);

 
/** Debug_Init() **
 * Initializes our debugging tools
 */
void Debug_Init(void){
	//PortF_Init();
	SysTick_Init();
}

/** Debug_Wait10Ms() **
 * Pause execution for 10ms
 * busy wait on systick
 */
void Debug_Wait10ms(void){
	SysTick_Wait10ms();
}

/** Debug_Switch1() **
 * Return the value of board switch 1
 */
bool Debug_Switch1(void){
	if(PF0){
		return true;
	} else{
		return false;
	}
}

/** Debug_Switch2() **
 * Return the value of board switch 2
 */
bool Debug_Switch2(void){
	if(PF4){
		return true;
	} else{
		return false;
	}
}

// Activates Port F For the Debugging tools
// We will use LEDS.
static void PortF_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGC2_R |= 0x20;
	delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;	//	unlock port f
	GPIO_PORTF_CR_R = 0x1F;
	GPIO_PORTF_PUR_R = 0x11;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_AMSEL_R = 0x00;
	GPIO_PORTF_AFSEL_R = 0x00;
	GPIO_PORTF_DEN_R = 0x1F;
}

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC; // enable SysTick with core clock
}
// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 12.5 nsec for 80 MHz clock)
void SysTick_Wait(uint32_t delay){
  volatile uint32_t elapsedTime;
  uint32_t startTime = NVIC_ST_CURRENT_R;
  do{ elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  } while(elapsedTime <= delay);
}
// Time delay using busy wait.
// This assumes 80 MHz system clock.
void SysTick_Wait10ms(){
  SysTick_Wait(800000);  // wait 10ms (assumes 80 MHz clock)
}

