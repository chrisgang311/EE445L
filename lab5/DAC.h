/** DAC.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 27th 2016
 * Description: Device Driver for our Digital to Analog converter
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
 
/** DAC_Init() **
 * Activate the DAC for voltage outputs
 * DAC Converts our digital output to an analog signal.
 * Outputs: none
 */
void DAC_Init(void);


/** DAC_Out() **
 * Write a digital output to SSI-DAC interface.  
 * Input: amplitude represented as a 12-bit number
 */
void DAC_Out(uint16_t amplitude);

/** DAC_Test() **
 * Test the output of a new DAC
 * Function is an endless profile that will never finish...
 */
void DAC_Test(uint16_t precision, uint16_t resolution);
