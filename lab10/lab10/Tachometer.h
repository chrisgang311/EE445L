/** Tachometer.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 9th 2016
 * Description: Drives the motor output with a PWM
 * PWM connected on PB6
 * Lab: 10
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer.h"
 
/** Measure Period **
 * Hook up the period measurement to PB7
 * Input is captured, and the period is the difference between
 * two rising edges
 */

/** Tachometer_Init() **
 * Activate the Tachometer for Input Loop.
 * Outputs: none
 */
void Tachometer_Init(void);

/** Tachometer_Read() **
 * Reads the latest period measurement with 0.1rps resolution
 * Outputs: latest period value
 */
uint32_t Tachometer_Read(void);
