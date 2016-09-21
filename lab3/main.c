/** lab3.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: Build an alarm clock
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 15th 2016
 *********************************************************************************/

/** hardware connections **
 * LCD Screen (ST7735R)
 * Backlight (pin 10) connected to +3.3 V
 * MISO (pin 9) unconnected 
 * SCK (pin 8) connected to PA2 (SSI0Clk)
 * MOSI (pin 7) connected to PA5 (SSI0Tx)
 * TFT_CS (pin 6) connected to PA3 (SSI0Fss)
 * CARD_CS (pin 5) unconnected
 * Data/Command (pin 4) connected to PA6 (GPIO)
 * RESET (pin 3) connected to PA7 (GPIO)
 * VCC (pin 2) connected to +3.3 V
 * Gnd (pin 1) connected to ground
 * 
 * Speaker
 * 
 * Keypad Tactile Switches
 * SW0-3 connected on PE0-3
 * 
 **************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "FSM.h"
#include "LCD.h"
#include "Clock.h"
#include "Keypad.h"
#include "Speaker.h"
#include "tm4c123gh6pm.h"


// debug flag
//#define DEBUG
#ifdef DEBUG
static void DebugInit(void);
// heartbeat port initializations
static void PortF_Init(void);
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF1 (*((volatile uint32_t *)0x40025008))
#endif // DEBUG

// switch debouncing flag
static bool ReadyForInput = true;

int main(void){
	// hardware initialization
	LCD_Init();
	Keypad_Init();
	Clock_Init();
	Speaker_Init();

	#ifdef DEBUG // alarm poll
	DebugInit();
	#endif // DEBUG
	
	Clock_Reset();
	// FSM alarm clock
	uint32_t state = 0x00, last = 0x00, keypad = 0x00;
	while(true){
		if(ReadyForInput){
			keypad = Keypad_Read();
			ReadyForInput = false;
		}
		if(keypad == last){
			// single pulse / idle
			state = UpdateClock(state, 0x00);
		} else{
			last = keypad;
			state = UpdateClock(state, keypad);
		}
	}

}

#ifdef DEBUG
/** DebugInit() **
 * Turn on debugging ports
 */
static void DebugInit(){
	PortF_Init();
	PF2 = 0x04;  // enter debug mode
}

// ***************** PortF_Init ****************
// Activate Port F for LED profiling
// Inputs:  none
// Outputs: none
void PortF_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
	delay = SYSCTL_RCGCTIMER_R;      			// allow time to finish activating
	GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000; // configure PF2 as GPIO
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
}

#endif // DEBUG

void Timer0A_Handler(void){
	// acknowledge timer0A timeout
  TIMER0_ICR_R = TIMER_ICR_TATOCINT; 
	static uint8_t uptick = 0;
	uptick = (uptick + 1) & 0x0F;
	if(uptick == 0x0F){
		Clock_Increment();
	}
	// switch debouncing
	ReadyForInput = true; 

	
	#ifdef DEBUG // sampling in handler
  PF2 ^= 0x04; // profile
  PF2 ^= 0x04; // profile
	// debug keypad values
	// static uint32_t idx = 0;
	//	if(idx) printf("%u.", Keypad_Read());
	//	else printf("%u.\n", Keypad_Read());
	//	idx = (idx + 1) % 7;
  PF2 ^= 0x04; // profile
	#endif // DEBUG
}

