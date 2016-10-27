/** Speaker.h **
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
 * Output goes on PE0
 */
 
#include <stdint.h>
#include "tm4c123gh6pm.h"
 
/** Speaker_Init() **
 * Activate the Speaker for Output processing.
 * Outputs: none
 */
void Speaker_Init(void);

/** Speaker_Play() **
 * Start playing a sound on the speaker
 * Speaker current will flow.
 */
void Speaker_Play(void);

/** Speaker_Mute() **
 * Mute the speaker
 * Speaker current will stop.
 */
void Speaker_Mute(void);

