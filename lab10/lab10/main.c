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
 #include "LCD.h"
 #include "PLL.h"
 #include "Plot.h"
 #include "Switch.h"

int main(void){ 
	uint32_t data;
  PLL_Init(Bus80MHz);   // 80 MHz
	LCD_Init(); // screen debugging
	
	Plot_Init("Motor Plot", 0, 4096);
  for(int idx = 0; ; idx++){
    data = 3000;
		Plot_PrintData(data);
		Plot_PlotData(data);
  }
}

