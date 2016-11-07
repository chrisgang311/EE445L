/** ADCContinuous.h
 * Author: Ronald Macmaster and Parth Adhia
 * Date: 11/03/2016
 *
 * Provide functions that initialize ADC0 SS3 
 * to use Continuous Sampling
 */

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

/** ADC0_InitContinous() **
 * Initialize ADC0 SEQ3 for continuous sampling
 * Output: none
 */
void ADC0_InitContinous();



/** ADC0_In() **
 * Read the ALREADY AVAILABLE ADC0 data.
 * Input: none
 * Output: 12-bit result of ADC conversion
 */
uint32_t ADC0_In(void);
