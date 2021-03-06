/** calib.h
 * Calibration data for our thermistor
 * 
 * Author: Ronald Macmaster and Parth Adhia
 * Date: 11/07/2016
 */
 
/** calibration data
 * R1 =	470k		R2 = 470k
 * R3	= 50k
 * 
 * calibration points (T, Rt)
 * First point:  ( , )
 * Second point: ( , )
 */
#include <stdint.h>

const uint16_t MAP_SIZE = 53;
uint16_t const ADCdata[53]={0,105,153,203,253,305,357,411,465,521,579,
     637,696,757,819,882,947,1013,1080,1148,1218,
     1289,1362,1436,1511,1588,1666,1746,1827,1909,1993,
     2079,2166,2254,2344,2436,2529,2623,2719,2817,2916,
     3016,3118,3221,3326,3432,3540,3649,3760,3872,3985,4095,4096};

uint16_t const Tdata[53]={4000,4000,3940,3880,3820,3760,3700,3640,3580,3520,3460,
     3400,3340,3280,3220,3160,3100,3040,2980,2920,2860,
     2800,2740,2680,2620,2560,2500,2440,2380,2320,2260,
     2200,2140,2080,2020,1960,1900,1840,1780,1720,1660,
     1600,1540,1480,1420,1360,1300,1240,1180,1120,1060,1000,1000};



