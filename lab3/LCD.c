/** LCD.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: LCD Methods for our Alarm Clock
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 14th 2016
 *********************************************************************************/

#include <stdint.h>
#include "ST7735.h"

static void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
static void ST7735_Circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);

/** Bitmap **
 * LCD BitMap data structure
 * Keeps track of bitmap metadata.
 * uint16_t x,y absolute position of bitmap
 * uint16_t h,w height and width of bitmap
 * const uint16_t *image ROM pointer to bitmap data
 */
typedef struct Bitmap Bitmap;
struct Bitmap{
	uint16_t x, y; // bitmap absolute position.
	uint16_t w, h; // bitmap dimensions
	const uint16_t *image; // bitmap data pointer.
};

/** LCD_Init() **
 * Set up the LCD Screen with red tabbing
 * Must call this function before calling any other LCD routines.
 */
void LCD_Init(){
	ST7735_InitR(INITR_REDTAB);
}

/** LCD_DrawLine() **
 * Forward function call to private ST7735 Function
 * inputs: x1, x2, y1, y2, color
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	ST7735_Line(x1, y1, x2, y2, color);
}

/** LCD_DrawBitMap() **
 * Draws a bitmap to the LCD Screen.
 */
void LCD_DrawBitmap(const uint16_t *image, uint16_t x, uint16_t y, uint16_t width, uint16_t height){
	ST7735_DrawBitmap(x, y, image, width, height);
}

 /** LCD_DrawCircle() **
 * Draws circle on the ST7735 color LCD
 * Inputs: (x,y) is the circle's center coordinate
 * radius the radius of the circle
 * color 16-bit color, which can be produced by ST7735_Color565()
 * Output: none
 */
void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color){
	ST7735_Circle(x, y, radius, color);
}

/** ST7735_DrawLine() **
 * Draws one line on the ST7735 color LCD
 * Inputs: (x1,y1) is the start point
 * 				 (x2,y2) is the end point
 * x1,x2 are horizontal positions, columns from the left edge
 * 					must be less than 128
 * 					0 is on the left, 126 is near the right
 * y1,y2 are vertical positions, rows from the top edge
 * 					must be less than 160
 * 					159 is near the wires, 0 is the side opposite the wires
 * color 16-bit color, which can be produced by ST7735_Color565()
 * Output: none
 */
static void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	int16_t rangeX = x2 - x1, rangeY = y2 - y1; 
	// draw a vertical line
	if(x1 == x2){
		if(y2 > y1){
			ST7735_DrawFastVLine(x1, y1, (y2 - y1), color); 
		}
		else {
			ST7735_DrawFastVLine(x2, y2, (y1 - y2), color); 
		} return;
	}
	// draw a horizontal line
	else if(y1 == y2){
		if(x2 > x1){
			ST7735_DrawFastHLine(x1, y1, (x2 - x1), color); 
		}
		else {
			ST7735_DrawFastHLine(x2, y2, (x1 - x2), color); 
		} return;
	}
	
	// diagonal line plotting
	if(x2 > x1){ // 2nd point on right
		for(int x = x1; x < x2; x++){
			uint16_t y = (rangeY * (x - x1) / rangeX) + y1; 
			ST7735_DrawPixel(x, y, color); 
		}
	}
	else if(x1 > x2){ // 2nd point on left
		for(int x = x2; x < x1; x++){
			uint16_t y = (rangeY * (x - x1) / rangeX) + y1; 
			ST7735_DrawPixel(x, y, color); 
		}
	}
}

 /** ST7735_DrawCircle() **
 * Draws circle on the ST7735 color LCD
 * Inputs: (x,y) is the circle's center coordinate
 * radius the radius of the circle
 * color 16-bit color, which can be produced by ST7735_Color565()
 * Output: none
 */
static void ST7735_Circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color){
	ST7735_DrawPixel(x, y, color); // plot center
}

