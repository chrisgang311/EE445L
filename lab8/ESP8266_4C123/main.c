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
#include "UART.h"
#include "esp8266.h"

#include "Keypad.h"
#include "LCD.h"
#include "Debug.h"
#include "Sensor.h"
#include "Speaker.h"

// prototypes for functions defined in startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

char Fetch[] = "GET /data/2.5/weather?q=Austin%20Texas&APPID=1234567890abcdef1234567890abcdef HTTP/1.1\r\nHost:api.openweathermap.org\r\n\r\n";
#define SERVER_POST "GET /post?city=Austin%%2C%%20Texas&user=ronny&content=ronny%20loves%20the%20esp8266 HTTP/1.1\r\nUser-Agent: Keil\r\nHost: server-144411.appspot.com\r\n\r\n"
// 1) go to http://openweathermap.org/appid#use 
// 2) Register on the Sign up page
// 3) get an API key (APPID) replace the 1234567890abcdef1234567890abcdef with your APPID
int WIFImain(void);
int main2(void);
int main(void){  
	//WIFImain();
	LCD_Init();
	Speaker_Init();
	Speaker_Play();
	while(1){	LCD_OutString("sound....\n");}
	
  DisableInterrupts();
  PLL_Init(Bus80MHz);
	LCD_Init();
	Keypad_Init();
	Debug_Init();
	LCD_OutString("Hello World!\n");
	LCD_OutString("Final Project!\n");
	
	uint32_t last = 0x00, keypad = 0x00;
	while(true){
		// sample and debounce the keypad
		keypad = Keypad_Read(); 
		Debug_Wait10ms(); 
		
		// change FSM state
		if(keypad == last){
			// single pulse / idle
			//state = PlayerFSM(state, 0x00);
		} else{
			last = keypad;
			char number[10];
			sprintf(number, "x%.4x\n", keypad);
			if(keypad != 0){LCD_OutString(number);}
			if(keypad&0x1111){LED_YellowOn();}
			if(keypad&0x2222){LED_YellowOff();}
			if(keypad&0xCCCC){LED_YellowToggle();}
			//state = PlayerFSM(state, keypad);
		}
	}	

}

int WIFImain(void){  
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  Debug_Init();  
	LCD_Init();
  UART_OutputInit();       // UART0 only used for debugging
  printf("\n\r-----------\n\rSystem starting...\n\r");
	LCD_OutString("start connect...\n");
  ESP8266_Init(115200);      // connect to access point, set up as client
  ESP8266_GetVersionNumber();
  while(1){
    ESP8266_GetStatus();
    if(ESP8266_MakeTCPConnection("server-144411.appspot.com")){ // open socket in server
      //LED_YellowOn();
			LCD_OutString("connecting to server...\n");
      ESP8266_SendTCP(SERVER_POST);
    }
    ESP8266_CloseTCPConnection();
		LCD_OutString("connection closed!\n");
    //while(Board_Input()==0){// wait for touch
    //}; 
    //LED_YellowOff();
    //LED_RedToggle();
  }
}

// transparent mode for testing
void ESP8266SendCommand(char *);
int main2(void){  char data;
  DisableInterrupts();
  PLL_Init(Bus80MHz);
	LCD_Init();
  Debug_Init(); 
  UART_OutputInit();       // UART0 as a terminal
    printf("\n\r-----------\n\rSystem starting at 115200 baud...\n\r");
//  ESP8266_Init(38400);
  ESP8266_InitUART(115200,true);
  ESP8266_EnableRXInterrupt();
  EnableInterrupts();
  ESP8266SendCommand("AT+RST\r\n");
  data = UART_InChar();
//  ESP8266SendCommand("AT+UART=115200,8,1,0,3\r\n");
//  data = UART_InChar();
//  ESP8266_InitUART(115200,true);
//  data = UART_InChar();
  
  while(1){
// echo data back and forth
    data = UART_InCharNonBlock();
    if(data){
      ESP8266_PrintChar(data);
    }
  }
}



