// ADCTestMain.c
// Runs on TM4C123
// This program periodically samples ADC channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
// Daniel Valvano
// September 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
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

// center of X-ohm potentiometer connected to PE3/AIN0
// bottom of X-ohm potentiometer connected to ground
// top of X-ohm potentiometer connected to +3.3V 
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "ADCSWTrigger.h"
#include "ST7735.h"
#include "PLL.h"

#define DEBUG
#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// timier initializations
void Timer0A_Init100HzInt(void);
void Timer1A_Init(void); 

// port initializations
void PortF_Init(void); 

volatile uint8_t recordFlag;
volatile uint32_t ADCvalue;
volatile uint32_t time[1000];
volatile uint32_t data[1000];

int main(void){
	// hardware initialization
  PLL_Init(Bus80MHz);
	PortF_Init();
	ST7735_InitR(INITR_REDTAB);
  ADC0_InitSWTriggerSeq3_Ch9(); 
  Timer0A_Init100HzInt(); // Timer0A has 100 Hz interrupts
	Timer1A_Init();	// Timer1A has Max countdown (53s)
	
	// sampling profile
	recordFlag = true;
	EnableInterrupts();
	PF2 = 0; // turn off LED
	printf("Started sampling...\n");
  while(recordFlag){
    PF1 = PF1 ^ 0x02;  // toggles while profiling
  } printf("Finished Sampling :)\n\n");

	
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
	
	#ifdef DEBUG // feedback
	printf("time[0] %u\n", time[0]);
	printf("data[0] %u\n", data[0]);
	printf("time[999] %u\n", time[999]);
	printf("data[999] %u\n", data[999]);
	printf("max: %u\nmin: %u\n", maxDiff, minDiff);
	printf("jitter: %u\n", jitter);
	#endif
	
	// plot ADC PMF
	/** algorithm: build an ADC frequency table
	 * find x and y ranges
	 * plot the function with ST7735 ploting
	 */
	
}

void Timer0A_Handler(void){
	// acknowledge timer0A timeout
  TIMER0_ICR_R = TIMER_ICR_TATOCINT; 
	static uint16_t idx = 0;
  PF2 ^= 0x04; // profile
  PF2 ^= 0x04; // profile
  
	// record ADC value
	DisableInterrupts();
	ADCvalue = ADC0_InSeq3();
	if(idx < 1000){
		time[idx] = TIMER1_TAR_R;
		data[idx] = ADCvalue;
		idx = idx + 1;
	}
	else{ // turn off sampling
		recordFlag = false;
		NVIC_DIS0_R = 1 << 19;
		TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
	}
	EnableInterrupts();
  PF2 ^= 0x04; // profile
}

// This debug function initializes Timer0A to request interrupts
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init100HzInt(void){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 799999;         // start value for 100 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}

// ***************** Timer1A_Init ****************
// Activate TIMER1 interrupts to countdown for 53s
// Inputs:  none (max countdown)
// Outputs: none
void Timer1A_Init(void){
  volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
  TIMER1_CTL_R = ~TIMER_CTL_TAEN;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, down-count 
  TIMER1_TAILR_R = 0xFFFFFFFF;  // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_CTL_R = TIMER_CTL_TAEN;    // 10) enable TIMER1A
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

