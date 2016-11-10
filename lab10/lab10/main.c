/** main.c
 * Author: Ronald Macmaster and Parth Adhia
 * Date: 11/09/2016
 * Lab 10:
 *
 * DC Motor Controller for lab 10.
 * Regulates a user input for RPM.
 */
 
 #include <stdio.h>
 #include <stdint.h>
 #include <stdbool.h>
 #include "LCD.h"
 #include "PLL.h"
 #include "Plot.h"
 #include "Switch.h"
 #include "Timer.h"
 #include "Debug.h"

// motor plot semaphore
bool redraw = true;
// motor plot speeds
uint16_t desired, actual;
int main(void){ 
  PLL_Init(Bus80MHz);   // 80 MHz
	LCD_Init(); // screen debugging
	desired = 0000; actual = 0000;
	
	
	Plot_Init("Motor Plot", 0, 5000);
	while(true){
		if(redraw){ // print motor plot
			redraw = false;
			Plot_PrintSpeed(desired, actual);
			Plot_PlotSpeed(actual);
		}
	}	
}




