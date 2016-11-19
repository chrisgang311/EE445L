/** WIFI.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 31st 2016
 * Description: WIFI Driver for the Alarm System
 * Uses the ESP8266
 * Lab: 8
 * TA: Dylan Zika
 * Date: October 31st 2016
 *********************************************************************************/
 
 #include <stdio.h>
 #include <stdbool.h>
 #include <stdint.h>
 #include <string.h>
 #include <stdlib.h>
 #include "tm4c123gh6pm.h"
 
 #include "esp8266.h"
 #include "UART.h"
 #include "pll.h"
 #include "LCD.h"
 
 // prototypes for functions defined in startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
 
/** WIFI_Init() 
 * Turn on the ESP and UART DEbugging on COM5
 * Return True if success connecting to WIFI
 * Return False if WIFI fails.
 */
bool WIFI_Init(){
	UART_OutputInit();       // UART0 only used for debugging
	ESP8266_Init(115200); // connect to access point, set up as client
	ESP8266_GetVersionNumber();
	return true;
}

/** WIFI_Send()
 * Sends an HTTP Payload to the host
 * returns true if payload was successfully sent.
 */
bool WIFI_Send(char *host, char *payload){
	ESP8266_GetStatus();
	bool success = false;
  if(ESP8266_MakeTCPConnection(host)){ // open socket in server
    success = ESP8266_SendTCP(payload);
  } ESP8266_CloseTCPConnection();
	return success;
}

// HTTP Data
#define SERVER_POST "GET /post?city=Austin%%2C%%20Texas&user=ronny&content=ronny%20loves%20his%20family. HTTP/1.1\r\nUser-Agent: Keil\r\nHost: server-144411.appspot.com\r\n\r\n"
int test1(void){  
  DisableInterrupts();
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
int test2(void){  char data;
	DisableInterrupts();
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


