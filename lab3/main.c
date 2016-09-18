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

#include "LCD.h"
#include "Clock.h"
#include "Keypad.h"
#include "Speaker.h"
#include "tm4c123gh6pm.h"


// debug flag
#define DEBUG
#ifdef DEBUG
static void DebugInit(void);
// heartbeat port initializations
static void PortF_Init(void);
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF1 (*((volatile uint32_t *)0x40025008))
#endif // DEBUG

// clock routines
static void DrawClock(void);

int main(void){
	// hardware initialization
	LCD_Init();
	Keypad_Init();
	Clock_Init();
	
	// Draw clock in center
	printf("-EE445L Alarm Clock-\n"); 
	DrawClock();

	#ifdef DEBUG // alarm poll
	DebugInit();
	while(true){
		Clock_Reset();
		Alarm_Set(10);
		while(Alarm_Read() != Clock_Read()){
			PF1 ^= 0x02;  // toggles while waiting for alarm
		} PF1 = 0;
		while(Keypad_Read() != 0x3){
			// spam alarm.
			Speaker_Expand();
			PF2 ^= 0x02;  
			Speaker_Contract();
		}
	}
	#endif // DEBUG
}


void Timer0A_Handler(void){
	// acknowledge timer0A timeout
  TIMER0_ICR_R = TIMER_ICR_TATOCINT; 
	Clock_Increment();
	#ifdef DEBUG // sampling in handler
	static uint16_t idx = 1;
  PF2 ^= 0x04; // profile
  PF2 ^= 0x04; // profile
	// debug keypad values
	if(idx) printf("%u.", Keypad_Read());
	else printf("%u.\n", Keypad_Read());
	idx = (idx + 1) % 7;
  PF2 ^= 0x04; // profile
	#endif // DEBUG
}

/** DrawClock() **
 * Center the clock in the middle of the screen
 */
static void DrawClock(){
	const uint16_t CLOCK_X = (LCD_WIDTH - CLOCK_WIDTH) / 2;
	const uint16_t CLOCK_Y = (LCD_HEIGHT + CLOCK_HEIGHT) / 2;
	LCD_DrawBitmap(CLOCK_IMAGE, CLOCK_X, CLOCK_Y, CLOCK_WIDTH, CLOCK_HEIGHT);
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


