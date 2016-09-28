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
#include "Keypad.h"
#include "mp3.h"


#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))

int main(void){ 
  PLL_Init(Bus80MHz); // bus clock at 80 MHz
}
