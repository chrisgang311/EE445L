/** Player.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 29th 2016
 * Description: Uses the timer hardware to implement a music player
 * utitilizes various timer interrupts
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 29th 2016
 *********************************************************************************/
 
 #include <stdint.h>
 #include "Timer.h"
 #include "mp3.h"
 #include "DAC.h"
 #include "Debug.h"
 
 // GPIO port F
#define PF1       (*((volatile uint32_t *)0x40025008))	// Tempo Interrupt
#define PF2       (*((volatile uint32_t *)0x40025010))	// Melody Interrupt
#define PF3       (*((volatile uint32_t *)0x40025020))	// Harmony Interrupt
 
 // song data
 static struct mp3 song;
 static uint32_t tempo;
 
 // song cursor
 static uint32_t cursor;
 
 /** Player_Init() **
 * Initialize the music player.
 * Set the song manager, and the tempo.
 */
void Player_Init(mp3 _song){
	// initialize the DAC
	DAC_Init();
	
	// test the DAC
	//DAC_Test(4096, 8);
	
	// song params
	song = _song;
	tempo = _song.tempo;
	cursor = 0; 
	
	// tempo controller
	Timer0A_Init(tempo, 1);
	
	// melody and bass
	Timer1A_Init(*(song.melody + cursor), 2);
	Timer2A_Init(*(song.harmony + cursor), 2);
	Timer3A_Init(*(song.bass + cursor), 2);
	
	// start music player
	Timer0A_Start();
	Timer1A_Start();
	Timer2A_Start();
	Timer3A_Start();
}

/** Player_Play() **
  * Start the music player
	*/
void Player_Play(){
	Timer0A_Arm();
	Timer1A_Arm();
	Timer2A_Arm();
	Timer3A_Arm();
}

/** Player_Pause() **
  * Stop the music player
	*/
void Player_Pause(){
	Timer0A_Disarm();
	Timer1A_Disarm();
	Timer2A_Disarm();
	Timer3A_Disarm();
}

/** Player_Rewind() **
  * Rewind the player
	*/
void Player_Rewind(){
	Timer1A_Period(*(song.melody + cursor));
	Timer2A_Period(*(song.harmony + cursor));
	Timer3A_Period(*(song.bass + cursor));
	Timer0A_Period(tempo);
	Timer0A_Acknowledge();
	cursor = 0;
}

/** Player_SetTempoMultiplier() **
  * Play the song at the new tempo
	*/
void Player_SetTempoMultiplier(float multiplier){
	tempo = (int)(song.tempo / multiplier);
	Timer0A_Period(tempo); // change tempo
}

/** Tempo controller **
 * Update the song cursor and change the notes played.
 * parses the mp3 song data.
 */
void Timer0A_Handler(){
	Timer0A_Acknowledge();
	PF1 ^= 0x02; PF1 ^= 0x02; // mark start
	cursor = (cursor + 1) % song.length;
	Timer1A_Period(*(song.melody + cursor));
	Timer2A_Period(*(song.harmony + cursor));
	Timer3A_Period(*(song.bass + cursor));
	PF1 ^= 0x02; // mark end
}


/** song EQ **/
static uint32_t melody = 0;
static uint32_t harmony = 0;
static uint32_t bass = 0;

/** Melody controller **
 * Update the instrument's cursor
 * Play the melody notes
 */
void Timer1A_Handler(){
	Timer1A_Acknowledge();
	static uint16_t mcursor = 0;
	uint16_t output = 0;
	PF2 ^= 0x04; PF2 ^= 0x04; // mark start
	melody = song.sound.melody[mcursor];
	mcursor = (mcursor + 1) % 32;
	output = (melody + harmony + bass) / 16;
	DAC_Out(output);
	PF2 ^= 0x04; // mark end
}

/** Harmony controller **
 * Update the instrument's cursor
 * Play the harmony notes
 */
void Timer2A_Handler(){
	Timer2A_Acknowledge();
	static uint16_t hcursor = 0;
	uint16_t output = 0;
	PF3 ^= 0x08; PF3 ^= 0x08; // mark start
	harmony = song.sound.harmony[hcursor];
	hcursor = (hcursor + 1) % 32;
	output = (melody + harmony + bass) / 16;
	DAC_Out(output);
	PF3 ^= 0x08;	// mark end
}

/** Bass controller **
 * Update the instrument's cursor
 * Play the bass notes
 */
void Timer3A_Handler(){
	Timer3A_Acknowledge();
	static uint16_t bcursor = 0;
	uint16_t output = 0;
	bass = song.sound.bass[bcursor];
	bcursor = (bcursor + 1) % 32;
	output = (melody + harmony + bass) / 16;
	DAC_Out(output);
}


