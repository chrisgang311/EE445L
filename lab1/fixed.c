/** fixed.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: August 26th 2016
 * Description: Fixed point output routines for our ST7735 LCD library extension.
 * Lab: 1
 * TA: Dylan Zika
 * Date: August 31st 2016
 * Hardware Configuration: Reference the ST7735.h header for classic LCD setup.
 *********************************************************************************/

/****************ST7735_sDecOut3***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
 Parameter LCD display
 12345    " *.***"
  2345    " 2.345"
 -8100    "-8.100"
  -102    "-0.102"
    31    " 0.031"
-12345    " *.***"
 ************************************************/
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
void ST7735_sDecOut3(int32_t n){
  const int32_t max = 9999;
  const int32_t min = -9999;
  char sign = ' ';

  // invalid number
  if(n < min || n > max){
    ST7735_OutString(" *.***");
    return;
  }
  // negative number
  else if(n < 0){
    sign = '-';
    n *= -1;
  }

  // print the number
  ST7735_OutChar(sign);
  ST7735_OutUDec((n / 1000) % 10);
  ST7735_OutChar('.');
  ST7735_OutUDec((n / 100) % 10);
  ST7735_OutUDec((n / 10) % 10);
  ST7735_OutUDec((n / 1) % 10);

}

/**************ST7735_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The ST7735_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
  Parameter LCD display
       0    "  0.00"
       2    "  0.01"
      64    "  0.25"
     100    "  0.39"
     500    "  1.95"
     512    "  2.00"
    5000    " 19.53"
   30000    "117.19"
  255997    "999.99"
  256000    "***.**"

 concept: ultimately output it as a decimal fixed-point with resolution 0.01.
 ************************************************/
void ST7735_uBinOut8(uint32_t n){
  const uint8_t length = 6;
  const uint8_t resolution = 2;
  const uint32_t max = 256000;
  char output[length]; // little endian output

  // invalid number
  if(n >= max){
    ST7735_OutString("***.**");
    return;
  }

  // fixed-point integer conversion
  // resolution 1/256 -> resolution 1/100
  n = ((n * 100)) >> 8;

  // decimal places
  for(int idx = 0; idx < resolution; idx++){
    output[idx] = (char)(n % 10 + '0');
    n = n / 10;
  }
  // one's digit & decimal pt.
  output[resolution] = '.';
  output[resolution + 1] = (char)(n % 10 + '0');
  n = n / 10;
  // ten's/hundred's and buffer
  for(int idx = resolution + 2; idx < length; idx++){
    output[idx] = n == 0 ? ' ' : (char)(n % 10 + '0');
    n = n / 10;
  }

  // print number
  for(int idx = length; idx > 0; idx--){
    ST7735_OutChar(output[idx - 1]);
  }

}

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
static int32_t Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0;
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){
  // clear screen.
  ST7735_FillScreen(0);
  ST7735_SetCursor(0, 0);
  ST7735_PlotClear(minY, maxY); 
  Xmin = minX; Xmax = maxX;
  Ymin = minY; Ymax = maxY;

  // print plot axes
  printf("%s \n", title);
  printf("x: (%d, %d)\n", minX, maxX);
  printf("y: (%d, %d)", minY, maxY);

}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]){

  for(int idx = 0; idx < num; idx++){
    // set coordinates in bounds
    int32_t x = bufX[idx], y = bufY[idx];
    x = x < Xmin ? Xmin : x;
    x = x > Xmax ? Xmax : x;
    y = y < Ymin ? Ymin : y;
    y = y > Ymax ? Ymax : y;

    // map point to LCD coordinate
    // y = Ymax maps to 32 : y = Ymin maps to 159
    x = (128 * (x - Xmin) / (Xmax - Xmin));
    y = (128 * (Ymax - y) / (Ymax - Ymin)) + 32;

    // plot point
    ST7735_DrawPixel(x, y, ST7735_RED);
    ST7735_DrawPixel(x + 1, y, ST7735_GREEN);
    ST7735_DrawPixel(x, y + 1, ST7735_BLACK);
    ST7735_DrawPixel(x + 1, y + 1, ST7735_BLUE);
  }

}

