/** Sensor.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: October 25th 2016
 * Description: Motion Sensor interface implementation.
 * Recieve and Handle an alarm notification from the motion sensor.
 * Lab: 7
 * TA: Dylan Zika
 * Date: October 25th 2016
 *********************************************************************************/
 
/** hardware connections **
 * SE-10 Motion sensor
 * Alarm edge input on PB7
 */
 
 #include <stdbool.h>
 
/** Sensor_Init() **
 * Activate the Motion Sensor for listening
 * Outputs: none
 */
void Sensor_Init(void);

/** Sensor_Reset() **
 * Resets the motion sensor flag.
 * Triggered status is cleared.
 */
void Sensor_Reset(void);

/** Sensor_Triggered() **
 * Check if the motion sensor was triggered
 * returns true if sensor was triggered
 */
bool Sensor_Triggered(void);
