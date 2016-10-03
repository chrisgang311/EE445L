/** Player.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 29th 2016
 * Description: Uses the timer hardware to implement a music player
 * utitilizes various timer interrupts
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 29th 2016
 *********************************************************************************/
 
 #include <stdint.h>
 #include "mp3.h"
 
/** Player_Init() **
 * Initialize the music player.
 * Set the song manager, and the tempo.
 */
void Player_Init(mp3 song);

/** Player_Play() **
  * Start the music player
	*/
void Player_Play(void);

/** Player_Pause() **
  * Stop the music player
	*/
void Player_Pause(void);

/** Player_Rewind() **
  * Rewind the player
	*/
void Player_Rewind(void);

/** Player_SetTempoMultiplier() **
* Play the song at the new speed : tempo * multiplier
	*/
void Player_SetTempoMultiplier(float multiplier);

