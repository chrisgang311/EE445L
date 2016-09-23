/** ADCSWTrigger.h **
 * Authors: Ronald Macmaster and Parth Adhia (Jonathan Valavno)
 * Created: September 21st 2016
 * Description: SW Sample the ADC for internal temperature data
   (modified valvano code)
 * Lab: 4
 * TA: Dylan Zika
 * Date: September 21st 2016
 *********************************************************************************/


// ADCSWTrigger.h
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

/** ACD0_Init() **
 * SW Triggered ADC Sampling.
 * Samples from PE4 on ADC0 SS3
 * maximum of 125K samples / sec
 * no gaurantee for real-time.
 *
 * for lab4: sample the internal temperature sensor
 */
void ADC0_Init(void);

//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC0_In(void);
