
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
#include "Timer.h"


#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))

int main(void){ 
  PLL_Init(Bus80MHz);              // bus clock at 50 MHz
  
}
