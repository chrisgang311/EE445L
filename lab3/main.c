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
static uint32_t UpdateClock(uint32_t state, uint32_t input);


int main(void){
	// hardware initialization
	LCD_Init();
	Keypad_Init();
	Clock_Init();
	
	/** read input
	 * display screen
	 * do alarm
	 * update state
	*/

	#ifdef DEBUG // alarm poll
	DebugInit();
	Clock_Reset();
	// FSM alarm clock
	uint32_t state = 0x00, last = 0x00;
	while(true){
		uint32_t keypad = Keypad_Read();
		if(keypad == last){
			// single pulse / idle
			state = UpdateClock(state, 0x00);
		} else{
			last = keypad;
			state = UpdateClock(state, keypad);
		}
	
	}
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

/** ClearClock() **
 * Clears the clock from the middle of the screen
 */
static void ClearClock(){
	const uint32_t COLOR_BLACK = 0x000000;
	const uint16_t CLOCK_X = (LCD_WIDTH - CLOCK_WIDTH) / 2;
	const uint16_t CLOCK_Y = (LCD_HEIGHT - CLOCK_HEIGHT) / 2;
	LCD_FillRectangle(CLOCK_X, CLOCK_Y, CLOCK_WIDTH, CLOCK_HEIGHT, COLOR_BLACK);
}

/** DrawDigital() **
 * Print the digital clock time string
 */
static void DrawDigital(){
	
}

/** Clock States **
 * States for Alarm Clock FSM
 * various alarm state handlers.
 */
static bool RedrawLCD = true; // redraw flag
static uint32_t MainScreen(uint32_t input);
static uint32_t ConfigureTime(uint32_t input);
static uint32_t ConfigureAlarm(uint32_t input);
static uint32_t ControlAlarm(uint32_t input);

/** UpdateClock() **
 * Process the user's input from the keypad.
 * Implements a mealy FSM.
 *
 * Alarm Clock States
 * 0x00 main screen
 * 0x01 configure time
 * 0x02 configure alarm
 * 0x03 play/mute alarm
 */
static uint32_t UpdateClock(uint32_t state, uint32_t input){
	switch(state){
		case 0x00:
			return MainScreen(input);
		case 0x01:
			return ConfigureTime(input);
		case 0x02:
			return ConfigureAlarm(input);
		case 0x03:
			return ControlAlarm(input);
		default:
			return MainScreen(input);
	}
}

/** MainScreen() **
 * Process the user's input from the keypad.
 * Main screen displays a digital or graphical clock.
 * Also displays some help controls.
 */
static void drawHourHand(uint8_t hours);
static void drawMinuteHand(uint8_t minutes);
static void drawSecondHand(uint8_t seconds);
static uint32_t MainScreen(uint32_t input){
	// read time
	static uint8_t digital = 0x00;
	static uint32_t oldTime = 0x000000;
	uint32_t time = Clock_Read();
	uint8_t minutes, hours, seconds;
	hours = (time & 0xFF0000) >> 16;
	minutes = (time & 0x00FF00) >> 8;
	seconds = (time & 0x0000FF) >> 0;
	
	if(RedrawLCD){
		LCD_SetCursor(0, 0);
		printf("-EE445L Alarm Clock-\n"); 
		printf("-By: Parth and Ronny\n");
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		printf("Press A to set time\n");
		printf("- B to set alarm\n");
		printf("- C to enable alarm");
		
		// draw clock
		if(digital){
			ClearClock();
			LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 3);
			printf("-Digital Mode-\n"); // banner
			LCD_SetCursor(CURSOR_WIDTH / 4, CURSOR_HEIGHT / 2);
			printf("- %.2d:%.2d:%.2d -", hours, minutes, seconds);
		} else{ // Draw clock in center
			DrawClock();
		} RedrawLCD = false;	
	}
	
	// decide next state.
	switch(input){
		case 0x00: // idle
			// update digital or graphical clock
			if(oldTime != time){
				if(digital){
					LCD_SetCursor(CURSOR_WIDTH / 4, CURSOR_HEIGHT / 2);
					printf("- %.2d:%.2d:%.2d -", hours, minutes, seconds);
				} else{ // graphical
					drawHourHand(hours);
					drawMinuteHand(minutes);
					drawSecondHand(seconds);
				} oldTime = time;
			} return 0x00; 
		case 0x01: // change display
			digital ^= 0x01;
			RedrawLCD = true;
			return 0x00; 
		case 0x08: // configure time
			LCD_Init();
			return 0x01;
		default:
			return 0x00;
	}

}

/** ConfigureTime() **
 * Process the user's input from the keypad.
 * Allows the user to set the current clock time.
 * Also displays some help controls.
 */
static uint32_t ConfigureTime(uint32_t input){
	// read time
	uint32_t time = Clock_Read();
	uint8_t minutes, hours, seconds;
	hours = (time & 0xFF0000) >> 16;
	minutes = (time & 0x00FF00) >> 8;
	seconds = (time & 0x0000FF) >> 0;
	
	if(RedrawLCD){
		LCD_SetCursor(0, 0);
		printf("-EE445L Alarm Clock-\n"); 
		printf("-By: Parth and Ronny\n");
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		printf("Press A to set time\n");
		printf("- B to set alarm\n");
		printf("- C to enable alarm");
		
//		// draw clock
//		if(digital){
//			ClearClock();
//			LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 3);
//			printf("-Digital Mode-\n"); // banner
//			LCD_SetCursor(CURSOR_WIDTH / 4, CURSOR_HEIGHT / 2);
//			printf("- %.2d:%.2d:%.2d -", hours, minutes, seconds);
//		} else{ // Draw clock in center
//			DrawClock();
//		} RedrawLCD = false;	
	}
	
	// decide next state.
	switch(input){
//		case 0x00: // idle
//			// update digital or graphical clock
//			if(oldTime != time){
//				if(digital){
//					LCD_SetCursor(CURSOR_WIDTH / 4, CURSOR_HEIGHT / 2);
//					printf("- %.2d:%.2d:%.2d -", hours, minutes, seconds);
//				} else{ // graphical
//					drawHourHand(hours);
//					drawMinuteHand(minutes);
//					drawSecondHand(seconds);
//				} oldTime = time;
//			} return 0x00; 
//		case 0x01: // change display
//			digital ^= 0x01;
//			RedrawLCD = true;
//			return 0x01; 
//		default:
//			return 0x00;
	}
}


/** ConfigureAlarm() **
 * Process the user's input from the keypad.
 * Allows the user to set the future alarm time.
 * Also displays some help controls.
 */
static uint32_t ConfigureAlarm(uint32_t input){
	return 0x00;
}

/** ConfigureTime() **
 * Process the user's input from the keypad.
 * Play or mute the speaker.
 * Display the on-board animation.
 * Also displays some help controls.
 */
static uint32_t ControlAlarm(uint32_t input){
	return 0x00;
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

void Timer0A_Handler(void){
	// acknowledge timer0A timeout
  TIMER0_ICR_R = TIMER_ICR_TATOCINT; 
	static bool uptick = false;
	if(uptick){
		Clock_Increment();
		uptick = false;
	}
	else{
		uptick = true;
	}
	#ifdef DEBUG // sampling in handler
	static uint16_t idx = 1;
  PF2 ^= 0x04; // profile
  PF2 ^= 0x04; // profile
	// debug keypad values
	//if(idx) printf("%u.", Keypad_Read());
	//else printf("%u.\n", Keypad_Read());
	//idx = (idx + 1) % 7;
  PF2 ^= 0x04; // profile
	#endif // DEBUG
}
#endif // DEBUG

void drawHourHand(uint8_t hour){
uint32_t x1 = LCD_WIDTH >> 1 ; 
uint32_t y1 = LCD_HEIGHT >> 1; 
uint32_t radius = CLOCK_WIDTH/2;  
// using the taylor series to find x2 and x1 
hour = hour % 12; 
float angle = hour * 3.14159 / 6; 
float sinangle = angle - ((angle*angle*angle)/6) + ((angle * angle * angle * angle * angle ) / 120) - ((angle * angle * angle * angle * angle * angle * angle )/5040) + (angle * angle * angle * angle * angle * angle * angle * angle * angle / 362880 ) - (angle * angle * angle * angle * angle * angle * angle * angle * angle * angle * angle / 39916800) ;
float cosangle =  1 - ((angle*angle)/2) + ((angle * angle * angle * angle) / 24) - ((angle * angle * angle * angle * angle * angle) / 720 ) + (angle * angle * angle * angle * angle * angle * angle * angle /40320) - (angle * angle * angle * angle * angle * angle * angle * angle * angle * angle / 3628800) ; 
uint32_t x2 =(uint32_t) (x1 + (radius * sinangle / 2)) ; 
uint32_t y2 = (uint32_t) (y1 - (radius * cosangle / 2 ));  

LCD_DrawLine(x1, y1, x2, y2, 0x00FF);

}  

void drawMinuteHand(uint8_t minutes){
uint32_t x1 = LCD_WIDTH >> 1 ; 
uint32_t y1 = LCD_HEIGHT >> 1; 
uint32_t radius = CLOCK_WIDTH/2; 
float angle = minutes * 3.14159 / 30; 
float sinangle = angle - ((angle*angle*angle)/6) + ((angle * angle * angle * angle * angle ) / 120) - ((angle * angle * angle * angle * angle * angle * angle )/5040) + (angle * angle * angle * angle * angle * angle * angle * angle * angle / 362880 ) - (angle * angle * angle * angle * angle * angle * angle * angle * angle * angle * angle / 39916800) ;
float cosangle =  1 - ((angle*angle)/2) + ((angle * angle * angle * angle) / 24) - ((angle * angle * angle * angle * angle * angle) / 720 ) + (angle * angle * angle * angle * angle * angle * angle * angle /40320) - (angle * angle * angle * angle * angle * angle * angle * angle * angle * angle / 3628800) ; 
uint32_t x2 = (uint32_t) (x1 + (radius * sinangle * 5 /8 )); 
uint32_t y2 = (uint32_t) (y1 - (radius * cosangle * 5 / 8));  

LCD_DrawLine(x1, y1, x2, y2, 0xFF00);

}

void drawSecondHand(uint8_t seconds){
uint32_t x1 = LCD_WIDTH >> 1 ; 
uint32_t y1 = LCD_HEIGHT >> 1; 
uint32_t radius = CLOCK_WIDTH * 5 / 16; 
float angle; 
if((seconds > 15 && seconds <= 30) || (seconds > 45 && seconds <= 60))
{
angle = 1.57 - ((seconds % 15) * 3.14159 / 30);
}
else
{
angle = ((seconds % 15) * 3.14159 / 30);
}

float sinangle = angle - ((angle*angle*angle)/6) + ((angle * angle * angle * angle * angle ) / 120) - ((angle * angle * angle * angle * angle * angle * angle )/5040) + (angle * angle * angle * angle * angle * angle * angle * angle * angle / 362880 ) - (angle * angle * angle * angle * angle * angle * angle * angle * angle * angle * angle / 39916800) ;
float cosangle =  1 - ((angle*angle)/2) + ((angle * angle * angle * angle) / 24) - ((angle * angle * angle * angle * angle * angle) / 720 ) + (angle * angle * angle * angle * angle * angle * angle * angle /40320) - (angle * angle * angle * angle * angle * angle * angle * angle * angle * angle / 3628800) ; 
uint32_t x2, y2; 
if(seconds <= 15)
{ 
x2 = (uint32_t) (x1 + (radius * sinangle)); 
y2 = (uint32_t) (y1 - (radius * cosangle));
}
else if(seconds > 15 && seconds <= 30) 
{
x2 = (uint32_t) (x1 + (radius * sinangle)); 
y2 = (uint32_t) (y1 + (radius * cosangle));
} 
else if(seconds > 30 && seconds <= 45) 
{
x2 = (uint32_t) (x1 - (radius * sinangle)); 
y2 = (uint32_t) (y1 + (radius * cosangle));
} 
else  
{
x2 = (uint32_t) (x1 - (radius * sinangle)); 
y2 = (uint32_t) (y1 - (radius * cosangle));
} 
LCD_DrawLine(x1, y1, x2, y2, 0xFFFF);

}
