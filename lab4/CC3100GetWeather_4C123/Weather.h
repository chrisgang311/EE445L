/** Weather.h **
 * Authors: Ronald Macmaster and Parth Adhia (Jonathan Valavno)
 * Created: September 21st 2016
 * Description: Weather model object for API response from openweathermap.org
 * Lab: 4
 * TA: Dylan Zika
 * Date: September 21st 2016
 *********************************************************************************/

#include <string.h>

/** Weather **
 * Weather model object from api.openweathermap.org response
 * JSON example:
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
#define FIELD_SIZE 50
struct Weather{
	char  type[FIELD_SIZE]; // type of weather.
	float tempurature; // tempurature in Celsius
	float wind; // wind speed in meters per second
	float clouds; // cloudiness %
	float rain; // rain volume (last 3h)
	
	// location
	char  city[FIELD_SIZE]; // name of city.
	float latitude; // latitude of city
	float longitude; // longitude of city
	
	uint32_t time; // time data was taken.
};
typedef struct Weather Weather;

/** Weather_Contstruct
 * Construct a new Weather object
 */
Weather Weather_Construct(char *wstring);

/** Weather_Contstruct
 * Construct a new Weather object
 */
void Weather_Print(Weather weather);
