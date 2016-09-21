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
 * Output goes on PB6
 */
 
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer.h"

// 80MHZ PWM frequencies with /16 divider
#define PWM_500Hz  9999
#define PWM_1000Hz 4999
#define PWM_5000Hz  999


// port init for Speaker
static void PWM_Init(uint32_t period, uint32_t duty);
	
/** Speaker_Init() **
 * Activate the Speaker for Output processing.
 * Outputs: none
 */
void Speaker_Init(){
	const uint32_t period = PWM_1000Hz / 2;
	PWM_Init(period, period / 2);
}

/** Speaker_Play() **
 * Start playing a sound on the speaker
 * Kick off the PWM on PB6
 * Speaker current will flow.
 */
void Speaker_Play(){
  PWM0_ENABLE_R |= 0x00000001;// enable PB6/M0PWM0
}

/** Speaker_Mute() **
 * Mute the speaker
 * Stop the PWM on PB6
 * Speaker current will stop.
 */
void Speaker_Mute(){
	PWM0_ENABLE_R &= ~0x00000001;  // disable PB6/M0PWM0
}

/** PWM_Init() **
 * period is 16-bit number of PWM clock cycles in one period (3<=period)
 * period for PB6 and PB7 must be the same
 * duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
 * PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
 *                = BusClock/2
 *                = 80 MHz/2 = 40 MHz (in this example)
 * Output on PB6/M0PWM0
 */
static void PWM_Init(uint32_t period, uint32_t duty){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){};
  GPIO_PORTB_AFSEL_R |= 0x40;           // enable alt funct on PB6
  GPIO_PORTB_PCTL_R &= ~0x0F000000;     // configure PB6 as PWM0
  GPIO_PORTB_PCTL_R |= 0x04000000;
  GPIO_PORTB_AMSEL_R &= ~0x40;          // disable analog functionality on PB6
  GPIO_PORTB_DEN_R |= 0x40;             // enable digital I/O on PB6
  SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
      ((SYSCTL_RCC_R & (~0x000E0000)) | 0x00060000);   //    configure for /2 divider
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PB6 goes low on LOAD
  // PB6 goes high on CMPA down
  PWM0_0_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_0_CMPA_R = duty - 1;             // 6) count value when output rises
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
//  PWM0_ENABLE_R |= 0x00000001;          // enable PB6/M0PWM0
}
