/** Plot.c
 * Plot for data acquisition system.
 * 
 * Author: Ronald Macmaster
 * Date: 11/07/2016
 */
 
 #include <stdint.h>
 #include <stdio.h>
 #include "ST7735.h"
 
 static void PrintFixed(uint16_t n);
 
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
	// clear screen.
  ST7735_FillScreen(0);
  ST7735_SetCursor(0, 0);
  ST7735_PlotClear(minData, maxData); 
  min = minData; max = maxData;

  // print plot axes
  printf("%s \n", title);
	printf("range:(");PrintFixed(min);
	printf(", ");PrintFixed(max);
	printf(")");
}
 
/** Plot_PrintData()
 * Print the data value in fixed point
 * Special place right above plot for data string
 */
void Plot_PrintData(uint16_t data){
	static uint8_t idx = 0;
	ST7735_SetCursor(0, 2);
	printf("temperature: ");
	PrintFixed(data);
	printf("C ");
	idx = idx + 1;
}

/** Plot_PlotData()
 * Plots the next data point in the sequence.
 * plot pointer is incremented one place
 */
void Plot_PlotData(uint16_t data){
	ST7735_PlotPoint(data);
	ST7735_PlotNextErase();
	ST7735_SetCursor(0, 3);
	PrintFixed(max);
	ST7735_SetCursor(0, 15);
	PrintFixed(min);
}

/****************PrintFixed(uint16_t n)***************
 converts fixed point number to LCD
 format unsigned 16-bit with resolution 0.01
 range 0 to +50.00
 Inputs:  unsigned 16-bit integer part of fixed-point number
 Outputs: none
 send exactly 5 characters to the LCD 
 Parameter LCD display
 12345    "*.***"
  2345    "23.45"
  3100    "31.00"
   102    "1.02"
    31    "0.31"
-12345    "*.***"
 ************************************************/
static void PrintFixed(uint16_t n){
  const uint16_t max = 5000;
  const uint16_t min = 0000;

  // invalid number
  if(n < min || n > max){
    ST7735_OutString(" *.***");
    return;
  }

	// build and print the number
	uint8_t idx = 0;
	char num[6] = {'\0'};
	if((n / 1000) % 10 > 0){ // ten's digit
		num[idx++] = ((n / 1000) % 10) + '0';
	} num[idx++] = ((n / 100) % 10) + '0';
	num[idx++] = '.';
	num[idx++] = ((n / 10) % 10) + '0';
	num[idx++] = ((n / 1) % 10) + '0';
	num[idx++] = '\0';
	ST7735_OutString(num);
}
