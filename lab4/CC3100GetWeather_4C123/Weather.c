/** Weather.c **
 * Authors: Ronald Macmaster and Parth Adhia (Jonathan Valavno)
 * Created: September 21st 2016
 * Description: Weather model object for API response from openweathermap.org
 * Lab: 4
 * TA: Dylan Zika
 * Date: September 21st 2016
 *********************************************************************************/

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "Weather.h"
//static void parseString(char *dest, const char *src, const char *field);

/*struct Weather{
	char *type; // type of weather.
	float tempurature; // tempurature in Celsius
	float wind; // wind speed in meters per second
	float clouds; // cloudiness %
	float rain; // rain volume (last 3h)
	
	// location
	char *city; // name of city.
	float latitude; // latitude of city
	float longitude; // longitude of city
	
	uint32_t time; // time data was taken.
};
 */

/** Weather_Contstruct
 * Construct a new Weather object from a json string
 * response comes from api.openweathermap.org
 * {"coord": {"lon":145.77,"lat":-16.92},
		"weather":[{"id":803,"main":"Clouds","description":"broken clouds","icon":"04n"}],
		"base":"cmc stations",
		"main":{"temp":293.25,"pressure":1019,"humidity":83,"temp_min":289.82,"temp_max":295.37},
		"wind":{"speed":5.1,"deg":150},
		"clouds":{"all":75},
		"rain":{"3h":3},
		"dt":1435658272,
		"sys":{"type":1,"id":8166,"message":0.0166,"country":"AU","sunrise":1435610796,"sunset":1435650870},
		"id":2172797,
		"name":"Cairns",
		"cod":200}
 */
Weather Weather_Construct(char *wstring){
	Weather weather; // new weather object
	
	// string data
	//printf("%p", wstring);
	sscanf(strstr(wstring, "\"main\":"), "\"main\":\"%s\",", weather.type);
	sscanf(strstr(wstring, "\"name\":"), "\"name\":\"%s\",", weather.city);
	
	
	int idx = 0; // null terminate weather type
	while(weather.type[idx] != '"' && weather.type[idx] != '\0'){
		idx = idx + 1;
	} weather.type[idx] = '\0';
	
	idx = 0; // null terminate city name
	while(weather.city[idx] != '"' && weather.city[idx] != '\0'){
		idx = idx + 1;
	} weather.city[idx] = '\0';
	
	// numerical data
	sscanf(strstr(wstring, "\"temp\":"), "\"temp\":%f,", &weather.tempurature);
	sscanf(strstr(wstring, "\"clouds\":"), "\"clouds\":{\"all\":%f}", &weather.clouds);
	sscanf(strstr(wstring, "\"speed\":"), "\"speed\":%f,", &weather.wind);
	sscanf(strstr(wstring, "\"rain\":"), "\"rain\":{\"3h\":%f}", &weather.rain);
	
	// location data
	sscanf(strstr(wstring, "{\"lon\":"), "{\"lon\":%f,\"lat\":%f},", &weather.longitude, &weather.latitude);
	sscanf(strstr(wstring, "\"dt\":"), "\"dt\":%u,", &weather.time);
	
	return weather;
}

/** Weather_Contstruct
 * print a weather object to stdout
 * like a toString method in Java
 */
void Weather_Print(Weather weather){
	// print location
	printf("city: %s\n", weather.city);
	printf("(lon,lat):\n  (%.2f, %.2f)\n\n", weather.longitude, weather.latitude);
	
	// print weather meta
	printf("weather: %s\n", weather.type);
	printf("time: %u\n", weather.time);
	
	// print weather data
	printf("temp: %.2f\n", weather.tempurature);
	printf("clouds: %.2f\n", weather.clouds);
	printf("wind: %.2f\n", weather.wind);
	printf("rain: %.2f\n", weather.rain);
	

	
	
}


/** parseString()
 * parse a string field from JSON string
 */
//static void parseString(char *dest, const char *src, const char *field){
//		// parse value from this format: "value",
//		src = strstr(src, field); // find field
//		src = strstr(src, ":\""); // find value start
//		char *value = strstr(src, "\",\"");	
//		if(src == NULL || field == NULL || value == NULL){
//			return;
//		} src = src + 2;
//		
//		// copy string value
//		int count = 0; // copy limit
//		while(src != value && count < FIELD_SIZE){
//			dest[count] = src[count];
//			count = count + 1;
//		} dest[count] = '\0';	
//}	




