/** Clock.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 14th 2016
 * Description: Alarm interface to abstract the Alarm Clock object.
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 14th 2016
 *********************************************************************************/
 
/** Clock_Init() **
 * Initialize the alarm clock (Plug in the alarm clock)
 * Must be called first
 */
void Clock_Init(void);

/** Clock_Reset() **
 * Reset the clock time.
 */
 void Clock_Reset(void);

/** Clock_Read() **
 * Read the clock time.
 */
uint32_t Clock_Read(void);

/** Clock_Set() **
 * Set the current clock time.
 */
void Clock_Set(uint32_t time);

/** Clock_Increment() **
 * Increment the clock time.
 */
void Clock_Increment(void);

/** Alarm_Set() ** 
 * Set the alarm
 */
void Alarm_Set(uint32_t time);

/** Alarm_Ready() ** 
 * Is the alarm on?
 */
bool Alarm_Ready(void);

/** Alarm_PowerOn() ** 
 * Turn on the alarm
 */
void Alarm_PowerOn(void);

/** Alarm_PowerOff() ** 
 * Turn off the alarm
 */
void Alarm_PowerOff(void);

/** Alarm_Read() ** 
 * Read the alarm value.
 */
uint32_t Alarm_Read(void);

/**** Clock Graphics Management ****/
/** Clock_DrawImage() **
 * Center the clock in the middle of the screen
 */
void Clock_DrawImage(void);

/** Clock_ClearImage() **
 * Clears the clock from the middle of the screen
 */
void Clock_ClearImage(void);


/** Clock_DrawHourHand() **
 * Plots the hours hand of the clock.
 * uses floating point trigonometry.
 */
void Clock_DrawHourHand(uint8_t hours, uint16_t color);

/** Clock_DrawMinuteHand() **
 * Plots the minutes hand of the clock.
 * uses floating point trigonometry.
 */
void Clock_DrawMinuteHand(uint8_t minutes, uint16_t color);

/** Clock_DrawSecondHand() **
 * Plots the seconds hand of the clock.
 * uses floating point trigonometry.
 */
void Clock_DrawSecondHand(uint8_t seconds, uint16_t color);

/** Clock picture */
#define CLOCK_WIDTH  80
#define CLOCK_HEIGHT 80
#define CLOCK_COLOR  0xC618
extern const uint16_t CLOCK_IMAGE[];
