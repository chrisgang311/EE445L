/** lab2.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 6th 2016
 * Description: Sample the ADC, and build a pmf / observe time-jitter measurements.
 * utitilizes various timer interrupts
 * Lab: 1
 * TA: Dylan Zika
 * Date: September 10th 2016
 *********************************************************************************/

/** hardware connections **
 * Backlight (pin 10) connected to +3.3 V
 * MISO (pin 9) unconnected 
 * SCK (pin 8) connected to PA2 (SSI0Clk)
 * MOSI (pin 7) connected to PA5 (SSI0Tx)
 * TFT_CS (pin 6) connected to PA3 (SSI0Fss)
 * CARD_CS (pin 5) unconnected
 * Data/Command (pin 4) connected to PA6 (GPIO)
 * RESET (pin 3) connected to PA7 (GPIO)
 * VCC (pin 2) connected to +3.3 V
 * Gnd (pin 1) connected to ground
 * center of X-ohm potentiometer connected to PE3/AIN0
 * bottom of X-ohm potentiometer connected to ground
 * top of X-ohm potentiometer connected to +3.3V 
 **************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "ADCSWTrigger.h"
#include "ST7735.h"
#include "PLL.h"
#include "Timer.h"

// debug flag
//#define DEBUG

// process managment
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// port initializations
void PortF_Init(void);
#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))

// data profiling
volatile uint32_t ADCvalue;
volatile static uint8_t recordFlag;
volatile static uint32_t time[1000];
volatile static uint32_t data[1000];
	
// pmf function (size 16 buckets)
#define PMF_SIZE 4096
#define PMF_THRESHOLD 5 // ignore noise outlier
volatile static uint16_t pmf[PMF_SIZE];
volatile static uint16_t Ymin, Ymax, PmfStart;
void CalculatePmf(void);
uint32_t CalculateJitter(void);

// line drawing
void DrawLines(void);
void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

int main(void){
	// hardware initialization
  PortF_Init();
	PLL_Init(Bus80MHz);
	ST7735_InitR(INITR_REDTAB);
  ADC0_InitSWTriggerSeq3_Ch9(); 
	Timer1A_Init(TIMER_MAXHz);	// Timer1A has Max countdown (53s)
	Timer0A_Init(TIMER_1000Hz); // Timer0A has 100 Hz interrupts
	
	// sampling profile
	recordFlag = true;
	PF2 = 0; // turn off ISR LED
	printf("Started sampling...\n");
	while(recordFlag){
		PF1 ^= 0x02;  // toggles while profiling
	} printf("Finished Sampling :)\n");
	
	#ifdef DEBUG // feedback
	// calculate jitter
	uint32_t jitter = CalculateJitter();
	printf("time[0] %u\n", time[0]);
	printf("data[0] %u\n", data[0]);
	printf("time[999] %u\n", time[999]);
	printf("data[999] %u\n", data[999]);
	printf("jitter: %u\n", jitter); 
	#endif
	
	#ifndef DEBUG // Plot PMF
	CalculatePmf();	// build pmf
	printf("Ymin: %u Ymax:%u\n", Ymin, Ymax);
	ST7735_PlotClear(Ymin, Ymax);
	for(int pdx = PmfStart; pdx < PmfStart + 128; pdx++){
		ST7735_PlotLine(pmf[pdx]); 
		ST7735_PlotNext();
	}
	
	// Part G) draw some lines
	DrawLines();
	#endif 
	
}

uint32_t CalculateJitter(){
	// calculate time jitter
	uint32_t minDiff = time[0] - time[1];
	uint32_t maxDiff = time[0] - time[1];
	for(int idx = 1; idx < 1000; idx++){
		// find min and max time diff
		uint32_t diff = time[idx - 1] - time[idx];
		if(diff < minDiff){
			minDiff = diff;
		}
		else if(diff > maxDiff){
			maxDiff = diff;
		}
	}
	uint32_t jitter = maxDiff - minDiff;
	return jitter;
}

void CalculatePmf(){
	/** algorithm: build an ADC frequency table
	 * find x and y ranges
	 * plot the function with ST7735 plot functions
	 */
	for(int idx = 0; idx < PMF_SIZE; idx++){
		pmf[idx] = 0;
	}
	// calculate pmf function and Y range
	uint16_t minY = pmf[0], maxY = pmf[0];
	for(int idx = 0; idx < 1000; idx++){
		uint32_t x = data[idx];
		pmf[x] = pmf[x] + 1;
		if(pmf[x] < minY){
			minY = pmf[x];
		}
		else if(pmf[x] > maxY){
			maxY = pmf[x];
			PmfStart = x - 64; // center around peak
			if(PmfStart > 4096){
				PmfStart = 0;
			}
		}
	}
	// set global y range
	Ymin = minY; Ymax = maxY;
}


void Timer0A_Handler(void){
	// acknowledge timer0A timeout
  TIMER0_ICR_R = TIMER_ICR_TATOCINT; 
	static uint16_t idx = 0;
  PF2 ^= 0x04; // profile
  PF2 ^= 0x04; // profile
  
	// record ADC value
	ADCvalue = ADC0_InSeq3();
	if(recordFlag && idx < 1000){
		time[idx] = TIMER1_TAR_R;
		data[idx] = ADCvalue;
		idx = idx + 1;
	}
	else{ // reset sampling
		idx = 0;
		recordFlag = false;
		Timer0A_Disarm();
		Timer0A_Stop();
	}
  PF2 ^= 0x04; // profile
}

// ***************** PortF_Init ****************
// Activate Port F for LED profiling
// Inputs:  none
// Outputs: none
void PortF_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
	delay = SYSCTL_RCGCTIMER_R;      			// allow time to finish activating
	GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000; // configure PF2 as GPIO
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
}

//************* ST7735_Line********************************************
// Draws one line on the ST7735 color LCD
// Inputs: (x1,y1) is the start point
// 				 (x2,y2) is the end point
// x1,x2 are horizontal positions, columns from the left edge
// 					must be less than 128
// 					0 is on the left, 126 is near the right
// y1,y2 are vertical positions, rows from the top edge
// 					must be less than 160
// 					159 is near the wires, 0 is the side opposite the wires
// color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
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

/** DrawLines() **
 * Few tests for our line drawing function.
 * Draw some vertical, horizontal, and diagno lines.
 */
void DrawLines(){
	// draw some straight lines
	uint16_t color = ST7735_RED;
	ST7735_Line(50, 100, 50, 50, color);
	ST7735_Line(100, 100, 100, 50, color);
	ST7735_Line(50, 50, 100, 50, color);
	ST7735_Line(50, 100, 100, 100, color);
	
	// diaganol lines
	ST7735_Line(100, 50, 50, 100, color);
	ST7735_Line(50, 100, 100, 50, color);
	ST7735_Line(100, 100, 50, 50, color);
	ST7735_Line(50, 50, 100, 100, color);
	
	// out of the park (remember to set the limits)
	ST7735_Line(100, 50, 50, 120, color);
	ST7735_Line(100, 100, 101, 95, color);
	ST7735_Line(25, 100, 101, 75, color);
}
