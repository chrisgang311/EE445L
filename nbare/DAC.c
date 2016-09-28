/** DAC.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 27th 2016
 * Description: Device Driver for our Digital to Analog converter
 * Runs on a TLV5616 DAC component
 * 4 pin SSI Interface
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
/** hardware connections **
 * DAC SSI Interface
 * Runs on SSI2					TLV5616 connection
 * PB4 SSI2CLK 					SClk
 * PB5 SSI2FSS					FS
 * PB6 SSI2RX						nothing
 * PB7 SSI2TX 					DIn
 */
 
 #include <stdint.h>
 #include "tm4c123gh6pm.h"
 
 //DAC hardward initialization
 static void PortB_Init(void);
 static void SSI2_Init(void);
 
/** DAC_Init() **
 * Activate the DAC for voltage outputs
 * DAC Converts our digital output to an analog signal.
 * Outputs: none
 */
void DAC_Init(void){
	PortB_Init();
	SSI2_Init(); 
}

/** DAC_Out() **
 * Write a digital output to SSI-DAC interface.  
 * Input: amplitude represented as a 12-bit number
 */
void DAC_Out(uint16_t amplitude){
	amplitude &= (0x0FFF); // right mask 12 bits
	while((SSI2_SR_R & SSI_SR_TNF) == 0){
		// wait till SSI TX FIFO is ready
	} SSI2_DR_R = amplitude; 
}

/** SSI2_Init() **
 * Run the SSI Clock at 10MHz
 * Transition and transmit SSI2Tx on rising clock edge.
 * Slave reads on the falling clock edge.
 * Frame select is first set to low, then MSB available at 1st clock edge (falling).
 * 
 * DAC Ts = 8ns, Th = 5ns. Don't clock the DAC or SSI over 20MHz!\
 * DAC Data: [15:12] XMPX   mode 0 = slow and 1 = fast   power 1 = power down and 0 = normal
* [11:0] New DAC value.  We should use slow mode and normal power
 */
static void SSI2_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGCSSI_R |= 0x04;     // 1) activate SSI2
	delay = SYSCTL_RCGCSSI_R; // allow time to finish activating
	SSI2_CR1_R = 0x00000000;  //2) Disable SSI, master mode
	SSI2_CPSR_R = 0x08;  //3) 10Mhz SSIClk Fssi = Fbus / (CPSDVSR * (1 + SCR))
	SSI2_CR0_R &= ~(0x0000FFFF);  //3) SCR = 0, Freescale frame format.
	SSI2_CR0_R |= 0x4F;  //5)) DSS = 12 bit data,  SPH = 0 and SP0 = 1 ( use 16 bit data??)
	SSI2_CR1_R |= SSI_CR1_SSE;  //4) enable SSI / set the SSE
}

/** PortB_Init() **
 * Standard port b init
 * Initialize with AFSEL for SSI2
 */
static void PortB_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02;     // 1) turn on port B
	delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
	GPIO_PORTB_AMSEL_R &= ~0xF0; // 2) disable analog on PB4-7
	GPIO_PORTB_AFSEL_R |= 0xF0; // 3) Enable alternative functionality on PB4-7
	GPIO_PORTB_PCTL_R &= ~0xFFFF0000;
	GPIO_PORTB_PCTL_R |= 0x22220000;// 4) choose GPIO functionality
	GPIO_PORTB_DIR_R |= 0xF0;   // 5) set PB7-4 to be outputs
	GPIO_PORTB_DEN_R |= 0xF0;    // 6) outputs are digital
}
