//***********************  main.c  ***********************
// Program written by:
// - Steven Prickett  steven.prickett@gmail.com
//
// Brief desicription of program:
// - Initializes an ESP8266 module to act as a WiFi client
//   and fetch weather data from openweathermap.org
//
//*********************************************************
/* Modified by Jonathan Valvano
 Feb 3, 2016
 Out of the box: to make this work you must
 Step 1) Set parameters of your AP in lines 59-60 of esp8266.c
 Step 2) Change line 39 with directions in lines 40-42
 Step 3) Run a terminal emulator like Putty or TExasDisplay at
         115200 bits/sec, 8 bit, 1 stop, no flow control
 Step 4) Set line 50 to match baud rate of your ESP8266 (9600 or 115200)
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "tm4c123gh6pm.h"

#include "pll.h"
#include "WIFI.h"

#include "Keypad.h"
#include "LCD.h"
#include "Debug.h"
#include "Alarm.h"
#include "FSM.h"

// prototypes for functions defined in startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// HTTP Data
#define SERVER_POST "GET /post?city=Austin%%2C%%20Texas&user=ronny&content=ronny%20loves%20his%20family. HTTP/1.1\r\nUser-Agent: Keil\r\nHost: server-144411.appspot.com\r\n\r\n"
int main(void){  
  DisableInterrupts();
  PLL_Init(Bus80MHz);
	LCD_Init();
	Keypad_Init();
	Alarm_Init();
	WIFI_Init();
	
	// debug // call tests here after WIFI_Init()
	Debug_Init(); // LEDS and Delay
	WIFI_Send("server-144411.appspot.com", SERVER_POST);

	uint32_t state = 0x00; // fsm state
	uint32_t last = 0x00, keypad = 0x00;
	while(true){
		// sample and debounce the keypad
		keypad = Keypad_Read(); 
		Debug_Wait10ms(); 
		
		// change FSM state
		if(keypad == last){
			// single pulse / idle
			state = UpdateFSM(state, 0x00);
		} else{
			last = keypad;
			state = UpdateFSM(state, keypad);
		}
	}	
}



