/** extra.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: August 31st 2016
 * Description: Extra Credit C Routines.
 * Lab: 1
 * TA: Dylan Zika
 * Date: August 31st 2016
 * Hardware Configuration: Reference the ST7735.h header for classic LCD setup.
 *********************************************************************************/

/** main **
 Kick off the extra credit routines one by one.
 Print out the test results to the screen.
 Test 1: floating point in C
 Test 2: fixed point in C
 Test 3: floating point in ASM
 Test 4: fixed point in ASM
 ************************************************/
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "fixed.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"

// Sys Tick regs
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

#define PF2   (*((volatile uint32_t *)0x40025010))
#define PF3   (*((volatile uint32_t *)0x40025020))
#define PF4   (*((volatile uint32_t *)0x40025040))

void Test1(void);
void Test2(void);
extern void Test3(void);
extern void Test4(void);
void SysTick_Init(void);
uint32_t SysTick_Time(void);
void Pause(void);
void PortF_Init(void);

int main() {
  PLL_Init(Bus80MHz);
  PortF_Init();
  SysTick_Init();
  ST7735_InitR(INITR_REDTAB);
  
  while(1){
    
    // test 1
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("Extra Credit: Test1\n");
    printf("C - Floating Point\n");
    for(int i = 0; i < 12; i++){
        uint32_t time = SysTick_Time();
        Test1();
        time = time - SysTick_Time();
      printf("trial %u: %u\n", i, time);
    }
    Pause();
    
		// test 1
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("Extra Credit: Test1\n");
    printf("C - Floating Point\n");
    for(int i = 12; i < 24; i++){
        uint32_t time = SysTick_Time();
        Test1();
        time = time - SysTick_Time();
      printf("trial %u: %u\n", i, time);
    }
    Pause();
		
    // test 2
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("Extra Credit: Test2\n");
    printf("C - Fixed Point\n");
    for(int i = 0; i < 12; i++){
        uint32_t time = SysTick_Time();
        Test2();
        time = time - SysTick_Time();
      printf("trial %u: %u\n", i, time);
    }
    Pause();

    // test 2
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("Extra Credit: Test2\n");
    printf("C - Fixed Point\n");
    for(int i = 12; i < 24; i++){
        uint32_t time = SysTick_Time();
        Test2();
        time = time - SysTick_Time();
      printf("trial %u: %u\n", i, time);
    }
    Pause();
    
    // test 3
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("Extra Credit: Test3\n");
    printf("ASM - Floating Point\n");
    for(int i = 0; i < 12; i++){
        uint32_t time = SysTick_Time();
        Test3();
        time = time - SysTick_Time();
      printf("trial %u: %u\n", i, time);
    }
    Pause();
		
		// test 3
    ST7735_FillScreen(0);
    ST7735_SetCursor(0, 0);
    printf("Extra Credit: Test3\n");
    printf("ASM - Floating Point\n");
    for(int i = 12; i < 24; i++){
        uint32_t time = SysTick_Time();
        Test3();
        time = time - SysTick_Time();
      printf("trial %u: %u\n", i, time);
    }
    Pause();
    
		// test 4
		ST7735_FillScreen(0);
		ST7735_SetCursor(0, 0);
		printf("Extra Credit: Test4\n");
		printf("ASM - Fixed Point\n");
		for(int i = 0; i < 12; i++){
				uint32_t time = SysTick_Time();
				Test4();
				time = time - SysTick_Time();
			printf("trial %u: %u\n", i, time);
		}
		Pause();
		
		// test 4
		ST7735_FillScreen(0);
		ST7735_SetCursor(0, 0);
		printf("Extra Credit: Test4\n");
		printf("ASM - Fixed Point\n");
		for(int i = 12; i < 24; i++){
				uint32_t time = SysTick_Time();
				Test4();
				time = time - SysTick_Time();
			printf("trial %u: %u\n", i, time);
		}
		Pause();
    
  }
  
}


// version 1: C floating point
// run with compiler options selected for floating-point hardware
volatile float T;// temperature in C
volatile uint32_t N; // 12-bit ADC value
void Test1(void){
  for(N = 0; N < 4096; N++){
    T = 10.0 + 0.009768 * N;
  }
}

// version 2: C fixed-point
void Test2(void){
  for(N = 0; N < 4096; N++){
    T = 1000 + (125 * N + 64) >> 7;
  }
}



// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC; // enable SysTick with core clock
}

// Read the systick
uint32_t SysTick_Time(void){
  return NVIC_ST_CURRENT_R;
}

// Subroutine to wait 10 msec
// Inputs: None
// Outputs: None
// Notes: ...
void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}

// wait for button press
void Pause(void){
  while(PF4==0x00){ 
    DelayWait10ms(10);
  }
  while(PF4==0x10){
    DelayWait10ms(10);
  }
}

// PF4 is input
// Make PF2 an output, enable digital I/O, ensure alt. functions off
void PortF_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x20;        // 1) activate clock for Port F
  while((SYSCTL_PRGPIO_R&0x20)==0){}; // allow time for clock to start
                                    // 2) no need to unlock PF2, PF4
  GPIO_PORTF_PCTL_R &= ~0x000F0F00; // 3) regular GPIO
  GPIO_PORTF_AMSEL_R &= ~0x14;      // 4) disable analog function on PF2, PF4
  GPIO_PORTF_PUR_R |= 0x10;         // 5) pullup for PF4
  GPIO_PORTF_DIR_R |= 0x04;         // 5) set direction to output
  GPIO_PORTF_AFSEL_R &= ~0x14;      // 6) regular port function
  GPIO_PORTF_DEN_R |= 0x14;         // 7) enable digital port
}

