/** main.c
 * UART runs at 115,200 baud rate 
 * Created By: Daniel Valvano
 * Created on: May 3, 2015
 * Modified By: Ronald Macmaster
 * Date: 11/03/2016
 *
 * ADC Sampling Driver to study the Nyquist Theorem.
 */

/* This example accompanies the books
  "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

"Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
 
 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

/** Preparation Questions:
 * 1) List three ways you could use to initiate the ADC conversion process.
   - Software Triggered (Processor triggered by default)
	 - Hardware Triggered off a GP Timer (Timer 0A)
	 - Continuous Sampling (continuously samples the ADC)
	 - many more (external GPIO Port, analog comparator, etc..)
 * 2) What is the way to know when the conversion process has been completed?
   - Once the conversion process is complete, 
			the ADC_RIS_R (raw interrupt status register) has flags that are set.
			(assuming the IE0 bit is also set).
 */

#include <stdint.h> 
#include <stdio.h>
#include "ADCHWTrigger.h"
#include "uart.h"
#include "PLL.h"
#include "Timer.h"
#include "LCD.h"
#include "Plot.h"

int main(void){ 
	uint32_t data;
  PLL_Init(Bus80MHz);   // 80 MHz
	LCD_Init(); // screen debugging
  UART_Init(); // initialize UART device
  ADC0_InitHWTrigger(TIMER_1000Hz);
	
	Plot_Init("Temperature plot", 0, 4096);
  for(int idx = 0; ; idx++){
    data = ADC0_In();
    UART_OutString("\n\rADC data ="); UART_OutUDec(data);
		LCD_SetCursor(0,0);
		Plot_PrintData(data);
		Plot_PlotData(data);
  }
}

