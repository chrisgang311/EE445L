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

// GPIO LED PortB
// RED LED ON PB2, YELLOW on PB3
#define PB0                     (*((volatile uint32_t *)0x40005004))
#define PB1                     (*((volatile uint32_t *)0x40005008))
#define PB2                     (*((volatile uint32_t *)0x40005010))
#define PB3                     (*((volatile uint32_t *)0x40005020))
#define PB4                     (*((volatile uint32_t *)0x40005040))
static void LED_Init(void);

// SysTick countdown
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value
static void SysTick_Init(void);
static void SysTick_Wait(uint32_t delay);
static void SysTick_Wait10ms(void);

 
/** Debug_Init() **
 * Initializes our debugging tools
 */
void Debug_Init(void){
	LED_Init();
	SysTick_Init();
}

/** Debug_Wait10Ms() **
 * Pause execution for 10ms
 * busy wait on systick
 */
void Debug_Wait10ms(void){
	SysTick_Wait10ms();
}

/** LED_Init() **
 * Initialize PortB for LED Output
 * LEDS on PB2 and PB3
 * Outputs: none
 */
void LED_Init(void){       
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02;    		 // turn on port B
	delay = SYSCTL_RCGCTIMER_R; 			 // allow time to finish activating
	GPIO_PORTB_AFSEL_R &= ~0x0C;        // disable alt funtion on PB2 and PB3
  GPIO_PORTB_PCTL_R &= ~0x0000FF00;
  GPIO_PORTB_AMSEL_R &= ~0x0C;       // disable analog functionality on PB2 and PB3
  GPIO_PORTB_DEN_R |= 0x0C;          // enable digital I/O on PB2 and PB3
	GPIO_PORTB_DIR_R |= 0x0C;					// Set PB2 PB3 as outputs 
}

//------------LED_RedOn------------
// Turn on red LED
// Input: none
// Output: none
void LED_RedOn(void){
  PB2 = 0x04;
}
//------------LED_RedOff------------
// Turn off red LED
// Input: none
// Output: none
void LED_RedOff(void){
  PB2 = 0x00;
}
//------------LED_RedToggle------------
// Toggle red LED
// Input: none
// Output: none
void LED_RedToggle(void){
  PB2 ^= 0x04;
}

//------------LED_YellowToggle------------
// Toggle yellow LED
// Input: none
// Output: none
void LED_YellowToggle(void){
  PB3 ^= 0x08;
}
//------------LED_YellowOn------------
// Turn on yellow LED
// Input: none
// Output: none
void LED_YellowOn(void){
  PB3 = 0x08;
}
//------------LED_YellowOff------------
// Turn off yellow LED
// Input: none
// Output: none
void LED_YellowOff(void){
  PB3 = 0x00;
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

