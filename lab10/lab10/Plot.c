/** Plot.c
 * Plot for data acquisition system.
 * Motor plot and user interface.
 * 
 * Author: Ronald Macmaster
 * Date: 11/07/2016
 */
 
 #include <stdint.h>
 #include <stdbool.h>
 #include <stdio.h>
 #include "ST7735.h"
 #include "Timer.h"
 #include "Switch.h"
 #include "Motor.h"
 
 // helper functions
void PrintFixed(uint16_t n);
 
 /**************Plot_Init()***************
 Specify the plot axes and range of LCD plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          min   smallest data value allowed, resolution= 0.01
          max   largest data value allowed, resolution= 0.01
 Outputs: none
*/
static int32_t min = 0, max = 0; 
void Plot_Init(char *title, int32_t minData, int32_t maxData){
	// setup data and timer
	min = minData; max = maxData;
	Timer1A_Init(TIMER_1Hz / 8, 0x02);
	Keypad_Init();
	
	// clear screen.
  ST7735_FillScreen(0);
  ST7735_SetCursor(0, 0);
	ST7735_PlotClear(min, max); 

  // print plot axes
  printf("%s \n", title);
	Timer1A_Start();
}
 
/** Plot_PrintSpeed()
 * Print the data value in fixed point
 * Special place right above plot for data string
 */
void Plot_PrintSpeed(uint16_t desired, uint16_t actual){
	// map input
	static uint8_t idx = 0;
	desired = desired;
	actual = actual;
	
	// print desired
	ST7735_SetCursor(0, 1);
	printf("desired: ");
	PrintFixed(desired);
	printf(" rps  ");
	
	// print actual
	ST7735_SetCursor(0, 2);
	printf("actual: ");
	PrintFixed(actual);
	printf(" rps  ");
	idx = idx + 1;
}

/** Plot_PlotSpeed()
 * Plots the next data point in the sequence.
 * plot pointer is incremented one place
 */
void Plot_PlotSpeed(uint16_t desired, uint16_t speed){
	speed = speed;
	ST7735_PlotPoint(desired, ST7735_RED);
	ST7735_PlotPoint(speed, ST7735_BLUE);
	ST7735_PlotNextErase();
	ST7735_SetCursor(0, 3);
	PrintFixed(max);
	ST7735_SetCursor(0, 15);
	PrintFixed(min);
}

/****************PrintFixed(uint16_t n)***************
 converts fixed point number to LCD
 format unsigned 16-bit with resolution 0.1
 range 0 to +500.0
 Inputs:  unsigned 16-bit integer part of fixed-point number
 Outputs: none
 Parameter LCD display
 12345    "*.**"
  2345    "234.5"
  2100    "210.0"
   120    "12.0"
    31    "3.1"
-12345    "*.*"
 ************************************************/
void PrintFixed(uint16_t n){
  const uint16_t max = 5000;
  //const uint16_t min = 0000;

  // invalid number
  if(/*n < min || */n > max){
    ST7735_OutString("*.*");
    return;
  }

	// build and print the number
	uint8_t idx = 0;
	char num[6] = {'\0'};
	// n >= 100.0
	if((n / 1000) % 10 > 0){ // hundred's digit
		num[idx++] = ((n / 1000) % 10) + '0';
		if((n / 100) % 10 > 0){ // ten's digit
			num[idx++] = ((n / 100) % 10) + '0';
		}
	} 
	// 10.0 <= n <= 100.0
	else if((n / 100) % 10 > 0){ // ten's digit
			num[idx++] = ((n / 100) % 10) + '0';
	} 
	
	// ones and tenth's digits
	num[idx++] = ((n / 10) % 10) + '0';
	num[idx++] = '.';
	num[idx++] = ((n / 1) % 10) + '0';
	num[idx++] = '\0';
	ST7735_OutString(num);
}


extern bool redraw;
void Timer1A_Handler(){
	Timer1A_Acknowledge();
	// sample and debounce the keypad
	static uint32_t keypad, last = 0x00;
	keypad = Keypad_Read(); 
	redraw = true;
	
	// update motor setting
	uint16_t desired = Motor_Desired();
	const uint16_t ds = 50;
	if(keypad != last){
		// increase desired speed
		last = keypad;
		if(keypad == 0x01){
			if(desired == 0){
				desired = min;
			} else if(desired + ds <= max){
				desired = desired + ds;
			} else{
				desired = max;
			}
		} 
		// decrease desired speed
		else if(keypad == 0x02){
			if(desired >= min + ds){
				desired = desired - ds;
			} else {
				//desired = min;
				desired = 0x00;
			}
		}
	} 
	// update desired speed
	Motor_SetDesired(desired);
}

