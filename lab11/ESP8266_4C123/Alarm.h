/** Alarm.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: October 31st 2016
 * Description: Top level wrapper for the speaker class.
 * Plays a siren instead of a quick sound.
 * Lab: 8
 * TA: Dylan Zika
 * Date: October 31st 2016
 *********************************************************************************/
 
 #include <stdbool.h>
 #include <stdint.h>
 
 #include "Timer.h"
 #include "Speaker.h"
 #include "Sensor.h"
 
 static bool triggered = false;
 static bool enabled = false;
 
 /** Alarm_Enable() **
 * Ready the Alarm for siren triggering
 * Outputs: none
 */
void Alarm_Init(void);

/** Alarm_Triggered() **
 * Check if the alarm is on / triggered
 */
bool Alarm_Triggered(void);

/** Alarm_Enable() **
 * Enable the Alarm completely
 * i.e. enable the sensor
 */
void Alarm_Enable(void);

/** Alarm_Disable() **
 * Disable the Alarm completely
 * i.e. turn off speaker and sensor
 */
void Alarm_Disable(void);

/** Alarm_Start() **
 * Trigger the alarm!
 */
void Alarm_Start(void);

/** Alarm_Stop() **
 * Pause the alarm!
 */
void Alarm_Stop(void);

/** Alarm picture */
#define ALARM_WIDTH  64
#define ALARM_HEIGHT 47
extern const uint16_t ALARM_IMAGE[];

/** Alarm picture */
#define WIFI_WIDTH  64
#define WIFI_HEIGHT 31
extern const uint16_t WIFI_IMAGE[];
