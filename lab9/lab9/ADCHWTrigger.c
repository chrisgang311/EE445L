/** ADCHWTrigger.c
 * Created By: Daniel Valvano
 * Created on: May 2, 2015
 * Modified By: Ronald Macmaster
 * Date: 11/03/2016
 *
 * Provide a function that initializes Timer0A to trigger ADC
 * SS3 conversions and request an interrupt when the conversion
 * is complete.
 */

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
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer.h"
#include "FIFO.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

static void PortE_Init(void);

// There are many choices to make when using the ADC, and many
// different combinations of settings will all do basically the
// same thing.  For simplicity, this function makes some choices
// for you.  When calling this function, be sure that it does
// not conflict with any other software that may be running on
// the microcontroller.  Particularly, ADC0 sample sequencer 3
// is used here because it only takes one sample, and only one
// sample is absolutely needed.  Sample sequencer 3 generates a
// raw interrupt when the conversion is complete, and it is then
// promoted to an ADC0 controller interrupt.  Hardware Timer0A
// triggers the ADC0 conversion at the programmed interval, and
// software handles the interrupt to process the measurement
// when it is complete.
//
// A simpler approach would be to use software to trigger the
// ADC0 conversion, wait for it to complete, and then process the
// measurement.
//
// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Timer0A: enabled
// Mode: 32-bit, down counting
// One-shot or periodic: periodic
// Interval value: programmable using 32-bit period
// Sample time is busPeriod*period
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: Timer0A
// SS3 1st sample source: programmable using variable 'channelNum' [0:11]
// SS3 interrupts: enabled and promoted to controller
void ADC0_InitHWTrigger(uint32_t period){
	PortE_Init();
	Timer0A_Init(period, 0);
	volatile uint32_t delay;
  SYSCTL_RCGCADC_R |= 0x01;     // activate ADC0 
  delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
	delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating

  ADC0_PC_R = 0x01;         // configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;    // sequencer 0 is highest, sequencer 3 is lowest
  ADC0_ACTSS_R &= ~0x08;    // disable sample sequencer 3
  ADC0_EMUX_R = (ADC0_EMUX_R&0xFFFF0FFF)+0x5000; // timer trigger event
  ADC0_SSMUX3_R = 9;
  ADC0_SSCTL3_R = 0x06;          // set flag and end                       
  ADC0_IM_R |= 0x08;             // enable SS3 interrupts
  ADC0_ACTSS_R |= 0x08;          // enable sample sequencer 3
	ADC0_SAC_R = ADC_SAC_AVG_64X;		// enable Hardware Averaging
  NVIC_PRI4_R = (NVIC_PRI4_R&0xFFFF00FF)|0x00004000; //priority 2
  NVIC_EN0_R = 1<<17;              // enable interrupt 17 in NVIC
	Timer0A_Start();
	EnableInterrupts();
}

/** ADC0_In() **
 * Read the ALREADY AVAILABLE ADC0 data.
 * Input: none
 * Output: 12-bit result of ADC conversion
 */
static volatile uint32_t ADCvalue = 0x0000;
uint32_t ADC0_In(void){
	while(FIFO_Empty()){/* wait for ADC data */}
	return FIFO_Read(); // read from data queue.
}

/** PortE_Init() **
 * Initialize PortE for ADC Sampling on PE4
 * PE4 is used for ADC0 Sequencer 3 input.
 * Outputs: none
 */
static void PortE_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x10;    // 1) turn on port E
	delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
	GPIO_PORTE_AMSEL_R |= 0x10;   // 2) enable analog on PE4
	GPIO_PORTE_AFSEL_R |= 0x10;   // 3) enable alternative functionality on PE4
	GPIO_PORTE_DIR_R &= ~0x10;    // 4) set PE4 to read inputs
	GPIO_PORTE_DEN_R &= ~0x10;    // 5) inputs are analog
}

void ADC0Seq3_Handler(void){
  ADC0_ISC_R = 0x08;          // acknowledge ADC sequence 3 completion
	ADCvalue = ADC0_SSFIFO3_R;  // 12-bit result 
	FIFO_Write(ADCvalue); // write to data queue.
}


