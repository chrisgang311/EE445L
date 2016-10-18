/** LCD.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: LCD Methods for our Alarm Clock
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 14th 2016
 *********************************************************************************/

#ifndef Bitmap
/** Bitmap **
 * LCD BitMap data structure
 * Keeps track of bitmap metadata.
 * uint16_t x,y absolute position of bitmap
 * uint16_t h,w height and width of bitmap
 * const uint16_t *image ROM pointer to bitmap data
 */
struct Bitmap;
typedef struct Bitmap Bitmap;
#endif

#define LCD_WIDTH 128
#define LCD_HEIGHT 160
#define CURSOR_WIDTH 20
#define CURSOR_HEIGHT 15

/** LCD_Init() **
 * Set up the LCD Screen with red tabbing
 * Must call this function before calling any other LCD routines.
 */
void LCD_Init(void);

/** LCD_DrawBitMap() **
 * Draws a bitmap to the LCD Screen with the 
 * provided dimensions
 * (x,y) is the lower left corner of the image.
 * 
 * input:
 * map.x x-location to draw bitmap
 * map.y y-location to draw bitmap
 * map.image rom pointer to bitmap image
 * map.w width of bitmap
 * map.h height of bitmap
 * output: none
 */
void LCD_DrawBitmap(const uint16_t *image, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/** LCD_FillRectangle() **
 * Draws a bitmap to the LCD Screen.
 */
void LCD_FillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
 
/** LCD_SetCursor() **
 * Resets the cursor to the desired location
 */
void LCD_SetCursor(uint16_t x, uint16_t y);
 
/** LCD_DrawLine() **
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
 void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
 
 /** LCD_DrawCircle() **
 * Draws circle on the ST7735 color LCD
 * Inputs: (x,y) is the circle's center coordinate
 * radius the radius of the circle
 * color 16-bit color, which can be produced by ST7735_Color565()
 * Output: none
 */
 void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);


 /** LCD_OutString() **
 * Outputs a string to the LCD
 */
void LCD_OutString(char *ptr);


