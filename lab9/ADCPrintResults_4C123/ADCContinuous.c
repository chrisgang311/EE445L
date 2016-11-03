/** ADCContinuous.c
 * Author: Ronald Macmaster and Parth Adhia
 * Date: 11/03/2016
 *
 * Provide functions that initialize ADC0 SS3 
 * to use Continuous Sampling
 */
 
#include <stdint.h>
#include "tm4c123gh6pm.h"
static void PortE_Init(void);

/** ADC0_InitContinous() **
 * Initialize ADC0 SEQ3 for continuous sampling
 * Output: none
 */
void ADC0_InitContinous(){ 
	PortE_Init();
  SYSCTL_RCGCADC_R |= 0x0001;     // 0) activate ADC0
  while((SYSCTL_PRADC_R&0x0001) != 0x0001){}; // good code, but not yet implemented in simulator
  ADC0_PC_R &= ~0xF;              // 1) clear max sample rate field
  ADC0_SSPRI_R = 0x3210;    			// 2) sequencer 3 is lowest
  ADC0_ACTSS_R &= ~0x0008;        // 3) disable sample sequencer 3
  ADC0_EMUX_R |=  0xF000;         // 4) seq3 is continous sample
  ADC0_SSMUX3_R &= ~0x000F;       // 5) clear SS3 field
  ADC0_SSMUX3_R += 9;             //    set channel
  ADC0_SSCTL3_R = 0x0004;         // 6) no TS0 D0 END0, yes IE0 
  ADC0_IM_R &= ~0x0008;           // 7) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 8) enable sample sequencer 3
}

/** ADC0_In() **
 * Read the ALREADY AVAILABLE ADC0 data.
 * Input: none
 * Output: 12-bit result of ADC conversion
 */
uint32_t ADC0_In(void){  
	uint32_t result;
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  return result;
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

