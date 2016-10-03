/** main.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 27th 2016
 * Description: driver file for our music player
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 27th 2016
 *********************************************************************************/
 
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdio.h>
#include "PLL.h"
#include "Debug.h"
#include "Switch.h"
#include "LCD.h"
#include "Player.h"
#include "mp3.h"

// Music Player FSM
static uint32_t PlayerFSM(uint32_t state, uint32_t input);

int main(void){ 
  PLL_Init(Bus80MHz); // bus clock at 80 MHz
	Debug_Init();
	LCD_Init();
	Keypad_Init();
	
	// initialize music player
	mp3 song = ZELDA;
	Player_Init(song);
	//Player_SetTempoMultiplier(1.5);
	printf("Now Playing ...\n");
	printf("%s", song.name);
	printf("\n\n");
	
	// controls
	printf("rewind : SW0 \n");
	printf("pause : SW1 \n");
	printf("change song : SW2 \n\n");
	
	printf("speedup : SW1 + SW2\n");
	printf("slowdown : SW1 + SW0\n");
	printf("reset : all at once\n");
	printf("\n L-R -> SW0.SW1.SW2");
	
	// Music Player FSM
	uint32_t state = 0x00, last = 0x00, keypad = 0x00;
	while(true){
		// sample and debounce the keypad
		keypad = Keypad_Read(); 
		Debug_Wait10ms(); 
		
		// change FSM state
		if(keypad == last){
			// single pulse / idle
			state = PlayerFSM(state, 0x00);
		} else{
			last = keypad;
			state = PlayerFSM(state, keypad);
		}
	}	
}

/** PlayerFSM **
 * perform music player FSM output
 * update music player state.
 */
// input table
#define SW0 0x01
#define SW1 0x02
#define SW2 0x04
// state table
#define PLAY 0x00
#define PAUSE 0x01
static int songno = 0;
static float speed = 1.0;
static uint32_t PlayState(uint32_t input);
static uint32_t PauseState(uint32_t input);
static uint32_t PlayerFSM(uint32_t state, uint32_t input){
	uint32_t rval = 0x00; // idle by defualt
	switch(state){
		case PLAY: // play
			Player_Play();
			rval = PlayState(input);
			break;
		case PAUSE: // pause
			Player_Pause();
			rval = PauseState(input);
			break;
	}
	return rval;
}


/** PlayState **
 * Play state of the FSM
 */
static uint32_t PlayState(uint32_t input){
	mp3 songs[3] = {ZELDA, POKEMON, CERULEAN};; // playlist
	switch(input){
		case SW1: // pause
			return PAUSE;
		case SW2: // rewind
			Player_Pause();
			//for(int i = 0; i < 100; i++) Debug_Wait10ms();
			Player_Rewind();
			return PLAY;
		case SW0: // switch songs
			songno = (songno + 1) % 3;
			Player_Init(songs[songno]);
			LCD_SetCursor(0,0);
			printf("Now Playing ...\n");
			printf("%s", songs[songno].name);
			return PLAY;
		case SW1 + SW2: // slow down
			speed = speed - 0.1f;
			Player_SetTempoMultiplier(speed);
			return PLAY;
		case SW1 + SW0: // speed up
			speed = speed + 0.1f;
			Player_SetTempoMultiplier(speed);
			return PLAY;
		case SW0 + SW1 + SW2: // reset speed
			speed = 1.0;
			Player_SetTempoMultiplier(speed);
			return PLAY;
		default: // play
			return PLAY;
	}
}

/** PauseState **
 * Pause state of the FSM
 */
static uint32_t PauseState(uint32_t input){
	mp3 songs[3] = {ZELDA, POKEMON, CERULEAN};; // playlist
	switch(input){
		case SW1: // play
			return PLAY;
		case SW2: // rewind
			Player_Pause();
			//for(int i = 0; i < 100; i++) Debug_Wait10ms();
			Player_Rewind();
			return PAUSE;
		case SW0: // switch songs
			songno = (songno + 1) % 3;
			Player_Init(songs[songno]);
			LCD_SetCursor(0,0);
			printf("Now Playing ...\n");
			printf("%s", songs[songno].name);
			return PLAY;
		case SW1 + SW2: // slow down
			speed = speed - 0.1f;
			Player_SetTempoMultiplier(speed);
			return PAUSE;
		case SW1 + SW0: // speed up
			speed = speed + 0.1f;
			Player_SetTempoMultiplier(speed);
			return PAUSE;
		case SW0 + SW1 + SW2: // reset speed
			speed = 1.0;
			Player_SetTempoMultiplier(speed);
			return PAUSE;
		default: // pause
			return PAUSE;
	}
}





