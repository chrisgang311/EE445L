/** Speaker.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: Speaker interface implementation.
 * Send a play or stop signal to the speaker to make a sound.
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 14th 2016
 *********************************************************************************/
 
/** hardware connections **
 * 32 Ohm Speaker
 * Output goes on PE0
 */
 
#include <stdint.h>
#include "tm4c123gh6pm.h"


// port E init for Speaker
static void PortE_Init(void);
#define PE0 (*((volatile uint32_t *)0x40024004))

/** Speaker_Init() **
 * Activate the Speaker for Output processing.
 * Outputs: none
 */
void Speaker_Init(){
	PortE_Init();
}

/** Speaker_Expand() **
 * Expand the speaker's diaphragm
 * Speaker current will flow.
 */
void Speaker_Expand(){
	PE0 = 0x01;
}

/** Speaker_Contract() **
 * Contract the speaker's diaphragm
 * Speaker current will stop.
 */
void Speaker_Contract(){
	PE0 = 0x00;
}

/** PortE_Init() **
 * Initialize PortE for Input Processing
 * Four Switch inputs: SW0-3 on PE1-4 
 * Outputs: none
 */
static void PortE_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x10;     // 1) turn on port E
	delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
	GPIO_PORTE_AMSEL_R &= ~0x01; // 2) disable analog on PE0-3
	GPIO_PORTE_AFSEL_R &= ~0x01; // 3) disable alternative functionality on PE0-3
	GPIO_PORTE_PCTL_R &= ~0x01;  // 4) choose GPIO functionality
	GPIO_PORTE_DIR_R |= 0x01;   // 5) set PE0-3 to read inputs
	GPIO_PORTE_DEN_R |= 0x1E;    // 6) inputs are digital
}
