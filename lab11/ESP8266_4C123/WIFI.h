/** WIFI.h **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 31st 2016
 * Description: WIFI Driver for the Alarm System
 * Uses the ESP8266
 * Lab: 8
 * TA: Dylan Zika
 * Date: October 31st 2016
 *********************************************************************************/
 
 #include <stdbool.h>
 
 /** WIFI_Init() 
 * Turn on the ESP and UART DEbugging on COM5
 * Return True if success connecting to WIFI
 * Return False if WIFI fails.
 */
 bool WIFI_Init(void);
 
/** WIFI_Send()
 * Sends an HTTP Payload to the host
 * returns true if payload was successfully sent.
 */
bool WIFI_Send(char *host, char *payload);
