/** ADCSWTrigger.c **
 * Authors: Ronald Macmaster and Parth Adhia (Jonathan Valavno)
 * Created: September 21st 2016
 * Description: SW Sample the ADC for internal temperature data
   (modified valvano code)
 * Lab: 4
 * TA: Dylan Zika
 * Date: September 21st 2016
 *********************************************************************************/

// ADCSWTrigger.c
// Runs on TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// Daniel Valvano
// August 6, 2015

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

/** ACD0_Init() **
 * SW Triggered ADC Sampling.
 * Samples from PE4 on ADC0 SS3
 * maximum of 125K samples / sec
 * no gaurantee for real-time.
 *
 * for lab4: sample the internal temperature sensor
 */
void ADC0_Init(void){
//  SYSCTL_RCGCGPIO_R |= 0x10;		// 1) activate clock for Port E
//  while((SYSCTL_PRGPIO_R&0x10) != 0x10){};
//  GPIO_PORTE_DIR_R &= ~0x10;      // 2) make PE4 input
//  GPIO_PORTE_AFSEL_R |= 0x10;     // 3) enable alternate function on PE4
//  GPIO_PORTE_DEN_R &= ~0x10;      // 4) disable digital I/O on PE4
//  GPIO_PORTE_AMSEL_R |= 0x10;     // 5) enable analog functionality on PE4
    
	SYSCTL_RCGCADC_R |= 0x0001;   // 0) activate ADC0 
  while((SYSCTL_PRADC_R&0x0001) != 0x0001){};    // good code, but not yet implemented in simulator
  ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
  ADC0_SSMUX3_R += 9;             //    set channel
  ADC0_SSCTL3_R = 0x000E;         // 12) yes TS0 IE0 END0 (set TS0 to sample the board's temperature sensor)
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
	//ADC0_SAC_R = ADC_SAC_AVG_64X;		// 14) enable Hardware Averaging 64x
	ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}


//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC0_In(void){  uint32_t result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
    // if you have an A0-A3 revision number, you need to add an 8 usec wait here
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}
