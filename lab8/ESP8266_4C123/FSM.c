/** FSM.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 31st 2016
 * Description: FSM Logic for main home alarm system
 * Lab: 8
 * TA: Dylan Zika
 * Date: October 31st 2016
 *********************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "Debug.h"
#include "FSM.h"
#include "LCD.h"
#include "Alarm.h"
#include "Keypad.h"
#include "tm4c123gh6pm.h"


/** Alarm States **
 * States for Home Alarm FSM
 * various alarm state handlers.
 */
static bool RedrawLCD = true; // redraw flag
uint32_t MainScreen(uint32_t input);
uint32_t SetPasscode(uint32_t input);
uint32_t EnterPasscode(uint32_t input);
uint32_t Alarm(uint32_t input);

 
/** UpdateFSM() **
 * Process the user's input from the keypad.
 * Implements a mealy FSM.
 *
 * System States
 * 0x00 main settings screen
 * 0x01 set passcode screen
 * 0x02 enter passcode screen
 * 0x03 alarm idle
 */
uint32_t UpdateFSM(uint32_t state, uint32_t input){
	switch(state){
		case 0x00:
			return MainScreen(input);
		case 0x01:
			return SetPasscode(input);
		case 0x02:
			return EnterPasscode(input);
		case 0x03:
			return Alarm(input);
		default:
			return MainScreen(input);
	}
}

/** MainScreen() **
 * Process the user's input from the keypad.
 * Main Screen displays settings navigation.
 * Also displays some help controls.
 */
static uint8_t backScreen = 0x00, forwardScreen = 0x00;
static uint16_t passcode[4] = {KEY_0, KEY_0, KEY_0, KEY_0};
uint32_t MainScreen(uint32_t input){
	if(RedrawLCD){
		RedrawLCD = false;
		LCD_SetCursor(0, 0);
		LCD_FillRectangle(0, 0, 128, 160, 0x0000);
		LCD_OutString(" -EE445L WIFI Alarm-\n"); 
		LCD_OutString(" By: Parth and Ronny\n");
		
		// debug password print
		char graphicalPasscode[5];
		graphicalPasscode[0] = KeyConvert(passcode[0]);
		graphicalPasscode[1] = KeyConvert(passcode[1]);
		graphicalPasscode[2] = KeyConvert(passcode[2]);
		graphicalPasscode[3] = KeyConvert(passcode[3]);
		graphicalPasscode[4] = '\0'; // null term string
		LCD_OutString(" passcode: ");
		LCD_OutString(graphicalPasscode);
		
		// picture
		LCD_DrawBitmap(WIFI_IMAGE, LCD_WIDTH / 4, LCD_HEIGHT * 2 / 3, WIFI_WIDTH, WIFI_HEIGHT);
		
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		LCD_OutString("Menu Controls:\n");
		LCD_OutString("*) set the passcode\n");
		LCD_OutString("#) enable alarm");
		
		// resource management
		Alarm_Stop();
		Alarm_Disable();
		LED_YellowOff();
	}
		
	// decide next state.
	switch(input){
		case 0x00: // idle
			return 0x00;
		case KEY_ASTERISK: // set the passcode
			RedrawLCD = true;
			return 0x01; 
		case KEY_POUND: // enter login screen
			RedrawLCD = true;
			backScreen = 0x00;
			forwardScreen = 0x03;
			return 0x02; 
		default:
			return 0x00;
	}
}

/** SetPasscode() **
 * Process the user's input from the keypad.
 * Allows the user to set the alarm's passcode.
 * Also displays some help controls.
 */
uint32_t SetPasscode(uint32_t input){
	static uint8_t field = 0; // passcode buffer
	static uint16_t code[4] = {KEY_0, KEY_0, KEY_0, KEY_0};
	if(RedrawLCD){
		RedrawLCD = false;
		LCD_SetCursor(0, 0);
		LCD_FillRectangle(0, 0, 128, 160, 0x0000);
		LCD_OutString("    -Set Passcode-\n"); 
		LCD_OutString(" By: Parth and Ronny\n");
		
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		LCD_OutString("Menu Controls:\n");
		LCD_OutString("*) backspace / home\n");
		LCD_OutString("#) submit passcode");

		// draw passcode setter
		LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 3);
		LCD_OutString("-Set Password-"); // banner
		LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 2);
		LCD_OutString("passcode: ---- ");
		field = 0; // reset	
	}	
	
	// decide next state.
	switch(input){
		case 0x00: // idle
			return 0x01; 
		case KEY_ASTERISK: // backspace / Return to Home Screen
			if(field == 0){
				RedrawLCD = true;	
				return 0x00;
			} 
			else{
				field = field - 1;
				LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 2);
				if(field == 0){
						LCD_OutString("passcode: ---- ");
				} else if(field == 1){
						LCD_OutString("passcode: *--- ");
				} else if(field == 2){
						LCD_OutString("passcode: **-- ");
				} else if(field == 3){
						LCD_OutString("passcode: ***- ");
				} return 0x01;
			}
		case KEY_POUND: // Submit Passcode
			if(field == 4){
				RedrawLCD = true;
				//set passcode
				passcode[0] = code[0];
				passcode[1] = code[1];
				passcode[2] = code[2];
				passcode[3] = code[3];
				return 0x00; // main screen
			} return 0x01; // do nothing
		case KEY_0: case KEY_1: case KEY_2: case KEY_3: 
		case KEY_4: case KEY_5: case KEY_6: case KEY_7: 
		case KEY_8: case KEY_9: case KEY_A: case KEY_B:
		case KEY_C: case KEY_D:
			// update code buffer
			if(field < 4){
				code[field] = input;
				field = field + 1;
				// update cursor
				LCD_SetCursor(CURSOR_WIDTH / 6 , CURSOR_HEIGHT / 2);
				if(field == 1){
						LCD_OutString("passcode: *--- ");
				} else if(field == 2){
						LCD_OutString("passcode: **-- ");
				} else if(field == 3){
						LCD_OutString("passcode: ***- ");
				} else if(field == 4){
						LCD_OutString("passcode: **** ");
				} 
			} return 0x01; // do nothing
		default:
			return 0x01;
	}
}


/** EnterPasscode() **
 * Process the user's input from the keypad.
 * Allows the user to login and set the alarm
 * Also displays some help controls.
 */
uint32_t EnterPasscode(uint32_t input){
	static uint8_t field = 0; // passcode buffer
	static uint16_t code[4] = {KEY_0, KEY_0, KEY_0, KEY_0};
	if(RedrawLCD){
		RedrawLCD = false;
		LCD_SetCursor(0, 0);
		LCD_FillRectangle(0, 0, 128, 160, 0x0000);
		LCD_OutString("    -Login Portal-\n"); 
		LCD_OutString(" By: Parth and Ronny\n");
		
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		LCD_OutString("Menu Controls:\n");
		LCD_OutString("*) backspace / home\n");
		LCD_OutString("#) submit passcode");

		// draw passcode setter
		LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 3);
		LCD_OutString("-Enter Passcode-"); // banner
		LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 2);
		LCD_OutString("passcode: ---- ");
		field = 0; // reset	
	}	
	
	// decide next state.
	switch(input){
		case 0x00: // idle
			return 0x02; 
		case KEY_ASTERISK: // backspace / Return to Home Screen
			if(field == 0){
				RedrawLCD = true;	
				return backScreen;
			} 
			else{
				field = field - 1;
				LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 2);
				if(field == 0){
						LCD_OutString("passcode: ---- ");
				} else if(field == 1){
						LCD_OutString("passcode: *--- ");
				} else if(field == 2){
						LCD_OutString("passcode: **-- ");
				} else if(field == 3){
						LCD_OutString("passcode: ***- ");
				} return 0x02;
			}
		case KEY_POUND: // Submit Passcode
			if(field == 4){
				RedrawLCD = true;
				//set passcode
				field = 0;
				if(passcode[0] == code[0] && passcode[1] == code[1] &&
					 passcode[2] == code[2] && passcode[3] == code[3]){
						LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT * 3 / 5);
						LCD_OutString("Good Job!\n"); // 1 sec delay
						if(backScreen == 0x00){
							LED_YellowOn();
							for(int i = 0; i < 200; i++) Debug_Wait10ms();
							LCD_SetCursor(CURSOR_WIDTH / 8, CURSOR_HEIGHT * 3 / 5);
							LCD_OutString("Enabling Alarm...\n"); // 1 sec delay
							LCD_OutString(" Get out of the way!"); // 1 sec delay
							for(int i = 0; i < 500; i++) Debug_Wait10ms();
							Alarm_Stop();
						} else{
							for(int i = 0; i < 100; i++) Debug_Wait10ms();
						}
						return forwardScreen; // next screen
				} else{
					LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT * 3 / 5);
					LCD_OutString("Wrong!"); // 1 sec delay
					for(int i = 0; i < 100; i++) Debug_Wait10ms();
					return 0x02; // same screen
				} 
			} return 0x02; // do nothing
		case KEY_0: case KEY_1: case KEY_2: case KEY_3: 
		case KEY_4: case KEY_5: case KEY_6: case KEY_7: 
		case KEY_8: case KEY_9: case KEY_A: case KEY_B:
		case KEY_C: case KEY_D:
			// update code buffer
			if(field < 4){
				code[field] = input;
				field = field + 1;
				// update cursor
				LCD_SetCursor(CURSOR_WIDTH / 6 , CURSOR_HEIGHT / 2);
				if(field == 1){
						LCD_OutString("passcode: *--- ");
				} else if(field == 2){
						LCD_OutString("passcode: **-- ");
				} else if(field == 3){
						LCD_OutString("passcode: ***- ");
				} else if(field == 4){
						LCD_OutString("passcode: **** ");
				} 
			} return 0x02; // same state
		default:
			return 0x02;
	}
}

/** Alarm() **
 * Enables the alarm and motion sensor.
 * Allows user to attempt alarm disable.
 */
uint32_t Alarm(uint32_t input){
	static bool started = false;
	if(RedrawLCD){
		RedrawLCD = false;
		LCD_SetCursor(0, 0);
		LCD_FillRectangle(0, 0, 128, 160, 0x0000);
		LCD_OutString(" -EE445L WIFI Alarm-\n"); 
		LCD_OutString(" By: Parth and Ronny\n");
		
		// Alarm is enabled
		// picture 
		if(!started){
			LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 4);
			LCD_OutString("Alarm is on :)");
		} else{
			LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 4);
			LCD_OutString("Intruder Alert!!");
		} LCD_DrawBitmap(ALARM_IMAGE, LCD_WIDTH / 4, LCD_HEIGHT * 2 / 3, ALARM_WIDTH, ALARM_HEIGHT);
		
		// print the help menu.
		LCD_SetCursor(0, CURSOR_HEIGHT * 7 / 8);
		LCD_OutString("Menu Controls:\n");
		LCD_OutString("#) disable alarm");
		
		// resource management
		Alarm_Enable();
		started = false;
		LED_YellowOn();
	}
	
	// kick off alarm
	if(Alarm_Triggered() && !started){
		Alarm_Start();
		started = true;
		LCD_SetCursor(CURSOR_WIDTH / 6, CURSOR_HEIGHT / 4);
		LCD_OutString("Intruder Alert!!");
	}
	
	// decide next state.
	switch(input){
		case 0x00: // idle
			return 0x03;
		case KEY_POUND: // enter login screen
			RedrawLCD = true;
			backScreen = 0x03;
			forwardScreen = 0x00;
			return 0x02; 
		default:
			return 0x03;
	}
}

