/** FSM.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 18th 2016
 * Description: FSM Logic for main clock program
 * Lab: 3
 * TA: Dylan Zika
 * Date: September 18th 2016
 *********************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "FSM.h"
#include "LCD.h"
#include "Clock.h"
#include "Speaker.h"
#include "tm4c123gh6pm.h"

/** Clock States **
 * States for Alarm Clock FSM
 * various alarm state handlers.
 */
static bool RedrawLCD = true; // redraw flag
uint32_t MainScreen(uint32_t input);
uint32_t ConfigureTime(uint32_t input);
uint32_t ConfigureAlarm(uint32_t input);
uint32_t ControlAlarm(uint32_t input);

/** UpdateClock() **
 * Process the user's input from the keypad.
 * Implements a mealy FSM.
 *
 * Alarm Clock States
 * 0x00 main screen
 * 0x01 configure time
 * 0x02 configure alarm
 * 0x03 play/mute alarm
 */
uint32_t UpdateClock(uint32_t state, uint32_t input){
	switch(state){
		case 0x00:
			return MainScreen(input);
		case 0x01:
			return ConfigureTime(input);
		case 0x02:
			return ConfigureAlarm(input);
		case 0x03:
			return ControlAlarm(input);
		default:
			return MainScreen(input);
	}
}

/** MainScreen() **
 * Process the user's input from the keypad.
 * Main screen displays a digital or graphical clock.
 * Also displays some help controls.
 */
uint32_t MainScreen(uint32_t input){
	// read time
	static uint8_t digital = 0x00;
	static uint32_t oldTime = 0x000000;
	uint32_t time = Clock_Read();
	uint8_t minutes, hours, seconds;
	hours = (time & 0xFF0000) >> 16;
	minutes = (time & 0x00FF00) >> 8;
	seconds = (time & 0x0000FF) >> 0;
	
	// check for alarm!
	static bool playAlarm = false;
	if(time == Alarm_Read() && Alarm_Ready()){
		Speaker_Play();
		playAlarm = true;
	} else if(!Alarm_Ready()){
		Speaker_Mute();
		playAlarm = false;
	}
	
	if(RedrawLCD){
		LCD_SetCursor(0, 0);
		printf("-EE445L Alarm Clock-\n"); 
		printf("-By: Parth and Ronny\n");
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		printf("Press A to set time\n");
		printf("- B to set alarm\n");
		printf("- C to enable alarm");
		
		LCD_SetCursor(0, 2);
		if(Alarm_Ready()){
			printf("-Alarm is <on> ");
		} else{
			printf("-Alarm is <off>");
		} 
		
		// draw clock
		if(digital){
			Clock_ClearImage();
			LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 3);
			printf("-Digital Clock-\n"); // banner
			LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
			printf("- %.2d:%.2d:%.2d -", hours, minutes, seconds);

		} else{ // Draw clock in center
			Clock_DrawImage();
		} RedrawLCD = false;	
	}
	
	// decide next state.
	switch(input){
		case 0x00: // idle
			// update digital or graphical clock
			if(oldTime != time){
				if(digital){
					LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
					printf("- %.2d:%.2d:%.2d -", hours, minutes, seconds);
				} else{ // graphical
					// clear old hands
					static uint8_t oldHours = 0, oldMinutes = 0, oldSeconds = 0;
					Clock_DrawSecondHand(oldSeconds, CLOCK_COLOR);
					Clock_DrawMinuteHand(oldMinutes, CLOCK_COLOR);
					Clock_DrawHourHand(oldHours, CLOCK_COLOR);
					
					// draw new hands
					Clock_DrawSecondHand(seconds, 0xFFFF);
					Clock_DrawMinuteHand(minutes, 0xFF00);
					Clock_DrawHourHand(hours, 0x00FF);
					oldHours = hours;
					oldMinutes = minutes;
					oldSeconds = seconds;
				} oldTime = time;
			} 
			// send to animation?
			if(playAlarm){
				LCD_FillRectangle(0, 0, 128, 160, 0x0000);
				RedrawLCD = true;
				return 0x03;
			} return 0x00;
		case 0x01: // change display
			digital ^= 0x01;
			RedrawLCD = true;
			return 0x00; 
		case 0x02: // toggle alarm
			if(Alarm_Ready()){
				Alarm_PowerOff();
			} else{
				Alarm_PowerOn();
			} LCD_SetCursor(0, 2);
			if(Alarm_Ready()){
				printf("-Alarm is <on> ");
			} else{
				printf("-Alarm is <off>");
			} return 0x00;
		case 0x04: // configure time
			LCD_Init();
			RedrawLCD = true;
			return 0x01;
		case 0x08: // configure Alarm
			LCD_Init();
			RedrawLCD = true;
			return 0x02;
		default:
			return 0x00;
	}

}

/** ConfigureTime() **
 * Process the user's input from the keypad.
 * Allows the user to set the current clock time.
 * Also displays some help controls.
 */
uint32_t ConfigureTime(uint32_t input){
	// 0 = hours, 1 = minutes, 2 = seconds, 3 = submit
	static uint8_t field = 0; 
	static uint32_t minutes, hours, seconds;
	
	if(RedrawLCD){
		LCD_SetCursor(0, 0);
		printf("-EE445L Alarm Clock-\n"); 
		printf("-By: Parth and Ronny\n");
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		printf("- A&B to move\n");
		printf("- C&D to +/-\n");
		printf("- C or D to submit");
		
		// read time
		uint32_t time = Clock_Read();
		hours = (time & 0xFF0000) >> 16;
		minutes = (time & 0x00FF00) >> 8;
		seconds = (time & 0x0000FF) >> 0;
		
		// draw clock setter
		LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 3);
		printf("-Configure Time-\n"); // banner
		LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
		printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);

		field = 0; // reset
		RedrawLCD = false;
	}
		
	// decide next state.
	switch(input){
		case 0x00: // idle
			return 0x01; 
		case 0x01: // increment current field
			switch(field){
				case 0: // hours
					hours = hours + 1;
					if(hours >= 24){
						hours = 0;
					} break;
				case 1: // minutes
					minutes = minutes + 1;
					if(minutes >= 60){
						minutes = 0;
					} break;
				case 2: // seconds
					seconds = seconds + 1;
					if(seconds >= 60){
						seconds = 0;
					} break;
				case 3: // submit
					LCD_Init();
					RedrawLCD = true;
					// set new time
					hours = (hours << 16);
					minutes = (minutes << 8);	
					seconds = (seconds << 0);
					Clock_Set(hours + minutes + seconds);
					return 0x00;
			}
			LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
			if(field == 2){
					printf("-%.2d:%.2d:<%.2d>-", hours, minutes, seconds);
			} else if(field == 1){
					printf("-%.2d:<%.2d>:%.2d-", hours, minutes, seconds);
			} else if(field == 0){
					printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);
			} return 0x01;
		case 0x02: // decrement current field
			switch(field){
				case 0: // hours
					if(hours == 0){
						hours = 23;
					} else{
						hours = hours - 1;
					} break;
				case 1: // minutes
					if(minutes == 0){
						minutes = 59;
					} else{
						minutes = minutes - 1;
					} break;
				case 2: // seconds
					if(seconds == 0){
						seconds = 59;
					} else{
						seconds = seconds - 1;
					} break;
				case 3: // submit
					LCD_Init();
					RedrawLCD = true;
					// set new time
					hours = (hours << 16);
					minutes = (minutes << 8);	
					seconds = (seconds << 0);
					Clock_Set(hours + minutes + seconds);
					return 0x00;
			}
			LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
			if(field == 2){
					printf("-%.2d:%.2d:<%.2d>-", hours, minutes, seconds);
			} else if(field == 1){
					printf("-%.2d:<%.2d>:%.2d-", hours, minutes, seconds);
			} else if(field == 0){
					printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);
			} return 0x01;
		case 0x04:
				if(field < 3){
					field = (field + 1);
				} else{
					field = 0;
				} 
				LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
				if(field == 3){
					printf("Update time?");
				} if(field == 2){
					printf("-%.2d:%.2d:<%.2d>-", hours, minutes, seconds);
				} else if(field == 1){
					printf("-%.2d:<%.2d>:%.2d-", hours, minutes, seconds);
				} else if(field == 0){
					printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);
				} return 0x01;
		case 0x08:
				LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
				if(field == 3){
					printf("-%.2d:%.2d:<%.2d>-", hours, minutes, seconds);
				} else if(field == 2){
					printf("-%.2d:<%.2d>:%.2d-", hours, minutes, seconds);
				} else if(field == 1){
					printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);
				}
				if(field > 0){
					field = (field - 1);
				} else{
					field = 0;
				} return 0x01;
		default:
			return 0x01;
	}
}


/** ConfigureAlarm() **
 * Process the user's input from the keypad.
 * Allows the user to set the future alarm time.
 * Also displays some help controls.
 */
uint32_t ConfigureAlarm(uint32_t input){
		// 0 = hours, 1 = minutes, 2 = seconds, 3 = submit
	static uint8_t field = 0; 
	static uint32_t minutes, hours, seconds;
	
	if(RedrawLCD){
		LCD_SetCursor(0, 0);
		printf("-EE445L Alarm Clock-\n"); 
		printf("-By: Parth and Ronny\n");
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		printf("- A&B to move\n");
		printf("- C&D to +/-\n");
		printf("- C or D to submit");
		
		// read alarm
		uint32_t alarm = Alarm_Read();
		hours = (alarm & 0xFF0000) >> 16;
		minutes = (alarm & 0x00FF00) >> 8;
		seconds = (alarm & 0x0000FF) >> 0;
		
		// draw alarm setter
		LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 3);
		printf("-Configure Alarm-\n"); // banner
		LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
		printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);

		field = 0; // reset
		RedrawLCD = false;
	}
		
	// decide next state.
	switch(input){
		case 0x00: // idle
			return 0x02; 
		case 0x01: // increment current field
			switch(field){
				case 0: // hours
					hours = hours + 1;
					if(hours >= 24){
						hours = 0;
					} break;
				case 1: // minutes
					minutes = minutes + 1;
					if(minutes >= 60){
						minutes = 0;
					} break;
				case 2: // seconds
					seconds = seconds + 1;
					if(seconds >= 60){
						seconds = 0;
					} break;
				case 3: // submit
					LCD_Init();
					RedrawLCD = true;
					// set new alarm
					hours = (hours << 16);
					minutes = (minutes << 8);	
					seconds = (seconds << 0);
					Alarm_Set(hours + minutes + seconds);
					return 0x00;
			}
			LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
			if(field == 2){
					printf("-%.2d:%.2d:<%.2d>-", hours, minutes, seconds);
			} else if(field == 1){
					printf("-%.2d:<%.2d>:%.2d-", hours, minutes, seconds);
			} else if(field == 0){
					printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);
			} return 0x02;
		case 0x02: // decrement current field
			switch(field){
				case 0: // hours
					if(hours == 0){
						hours = 23;
					} else{
						hours = hours - 1;
					} break;
				case 1: // minutes
					if(minutes == 0){
						minutes = 59;
					} else{
						minutes = minutes - 1;
					} break;
				case 2: // seconds
					if(seconds == 0){
						seconds = 59;
					} else{
						seconds = seconds - 1;
					} break;
				case 3: // submit
					LCD_Init();
					RedrawLCD = true;
					// set new alarm
					hours = (hours << 16);
					minutes = (minutes << 8);	
					seconds = (seconds << 0);
					Alarm_Set(hours + minutes + seconds);
					return 0x00;
			}
			LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
			if(field == 2){
					printf("-%.2d:%.2d:<%.2d>-", hours, minutes, seconds);
			} else if(field == 1){
					printf("-%.2d:<%.2d>:%.2d-", hours, minutes, seconds);
			} else if(field == 0){
					printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);
			} return 0x02;
		case 0x04:
				if(field < 3){
					field = (field + 1);
				} else{
					field = 0;
				} 
				LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
				if(field == 3){
					printf("Update Alarm?");
				} if(field == 2){
					printf("-%.2d:%.2d:<%.2d>-", hours, minutes, seconds);
				} else if(field == 1){
					printf("-%.2d:<%.2d>:%.2d-", hours, minutes, seconds);
				} else if(field == 0){
					printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);
				} return 0x02;
		case 0x08:
				LCD_SetCursor(CURSOR_WIDTH / 5, CURSOR_HEIGHT / 2);
				if(field == 3){
					printf("-%.2d:%.2d:<%.2d>-", hours, minutes, seconds);
				} else if(field == 2){
					printf("-%.2d:<%.2d>:%.2d-", hours, minutes, seconds);
				} else if(field == 1){
					printf("-<%.2d>:%.2d:%.2d-", hours, minutes, seconds);
				}
				if(field > 0){
					field = (field - 1);
				} else{
					field = 0;
				} return 0x02;
		default:
			return 0x02;
	}
}

/** ControlAlarm() **
 * Plays the alarm and animation
 */
uint32_t ControlAlarm(uint32_t input){
	// 0 = hours, 1 = minutes, 2 = seconds, 3 = submit
	static uint8_t field = 0; 
		
	// decide next state.
	switch(input){
		case 0x00: // idle
			// animation goes here
			if(field >= 6 && field < 22){
				printf("            Alarm!!!\n");
			} else{
				printf("Alarm!!!            \n");
			} field = (field + 1) & 0x1F;
			return 0x03; 
		case 0x01: case 0x02: case 0x04: case 0x08:
			LCD_Init();
			field = 0;
			RedrawLCD = true;
			if(Alarm_Ready()){
				Alarm_PowerOff();
			} else{
				Alarm_PowerOn();
			} return 0x00;
		default:
			return 0x00;
	}
}
