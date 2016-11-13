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
 #include "Motor.h"
 #include "Debug.h"
 #include "Tachometer.h"

// motor plot semaphore
bool redraw = true;
// motor plot speeds
uint16_t desired = 450, actual = 0;
int main(void){ 
  PLL_Init(Bus80MHz);   // 80 MHz
	LCD_Init(); // screen debugging
	uint16_t min = 350, max = 550;	
	uint32_t period = 40000;
	
	Motor_Init(period, (period * 4) / 10, period);
	Motor_Start(); Motor_SetDesired(desired);
	Plot_Init("Motor Plot", min, max);
	while(true){
		if(redraw){ // print motor plot
			redraw = false;
			actual = Motor_Actual();
			desired = Motor_Desired();
			Plot_PrintSpeed(desired, actual);
			Plot_PlotSpeed(desired, actual);
		}
	}	
}




