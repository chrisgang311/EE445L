/** main.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 27th 2016
 * Description: driver file for our music player
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
#include "Timer.h"
#include "Switch.h"
#include "mp3.h"


#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
	

// periods calculated for tempo at 144 bpm
// beat period = (60s/tempo)*bus speed 
#define sixteenth_note 6750000
#define eigth_note 13500000
#define quarter_note 27000000
#define half_note 54000000

// slower 120bpm
//#define sixteenth_note 10000000
//#define eigth_note 20000000
//#define quarter_note 40000000
//#define half_note 80000000

int main(void){ 
  PLL_Init(Bus80MHz); // bus clock at 80 MHz
}

// ************Debug_Init**********************
// Activates Port F For the Debugging tools
// We will use LEDS.
void Debug_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGC2_R |= 0x20;
	delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;	//	unlock port f
	GPIO_PORTF_CR_R = 0x1F;
	GPIO_PORTF_PUR_R = 0x11;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_AMSEL_R = 0x00;
	GPIO_PORTF_AFSEL_R = 0x00;
	GPIO_PORTF_DEN_R = 0x1F;
}

