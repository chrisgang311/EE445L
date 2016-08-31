#include <stdint.h>
#include "fixed.h"
#include "ST7735.h"

char decerror[] = "*.***"; 
char binerror[] = "***.**";
char decdisplayvalue[7]; 
char bindisplayvalue[6];
int32_t xlow = 0; 
int32_t ylow = 0; 
int32_t xhigh = 0; 
int32_t yhigh = 0; 
void ST7735_sDecOut3(int32_t n)
{
	if(n <= 9999 && n >= -9999)
	{
		if(n < 0)
		{
			decdisplayvalue[0] = '-'; 
			n = n * -1 ; 
		}
		else
		{
			decdisplayvalue[0] = ' '; 
		}
		uint32_t number = n; 
		decdisplayvalue[6] ='\0'; 
		for(uint8_t loopvar = 5; loopvar >= 1; loopvar --)
		{ 
			if(loopvar != 2)
			{
				decdisplayvalue[loopvar] = (char) ((number % 10) + '0') ; 
				number  = number / 10; 
			}
			else
			{
				decdisplayvalue[loopvar] = '.'; 
			}
		}
	ST7735_OutString(decdisplayvalue);
	}
	else
	{
		ST7735_OutString(decerror);
	}
}


void ST7735_uBinOut8(uint32_t n) 
{
	if(n < 256000 && n > 0)
	{
		uint32_t number = n/256; 
		bindisplayvalue[5] ='\0'; 
		uint32_t poweroften = 100;
		uint8_t boolcanplace0 = 0; 
		uint32_t pointremainder = (n%256)*10; 
		for(uint8_t loopvar = 0; loopvar < 6; loopvar ++)
		{ 
			 
			if(loopvar != 3)
			{
				if(loopvar < 3) 
				{
					if(number / poweroften > 0)
					{
						bindisplayvalue[loopvar] = (char) ((number / poweroften) + '0') ; 
						boolcanplace0 = 1; 
					}
					else if( number / poweroften == 0 && boolcanplace0 == 1)
					{
						bindisplayvalue[loopvar] = (char) ('0');
					}
					else
					{
						if( n < 256 && loopvar == 2) 
						{
							bindisplayvalue[loopvar] = (char) ('0');
						}
						else
						{
						bindisplayvalue[loopvar] = ' ';
						}
					}
					number  = number % poweroften; 
					poweroften = poweroften / 10; 
			}
				else
				{
					if(pointremainder < 256)
					{
						bindisplayvalue[loopvar] = (char) ('0'); 
						pointremainder = pointremainder * 10; 
				}
					else
					{	
						bindisplayvalue[loopvar] = (char) ((pointremainder / 256) + '0') ; 
						pointremainder = (pointremainder % 256 ) * 10; 
					}
			}
		}
			else
			{
				bindisplayvalue[loopvar] = '.'; 
			}
		}
	ST7735_OutString(bindisplayvalue);
	}
	else
	{
		if(n == 0)
		{
		ST7735_OutString("  0.00"); 
		}
		else
		{
		ST7735_OutString(binerror);
		}
	}
}
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY)
{
ST7735_FillScreen(0); 
ST7735_SetCursor(0,0);	
ST7735_PlotClear(minY, maxY); 
ST7735_OutString(title); 
ST7735_OutString("\n");
xlow = minX; ylow = minY; xhigh = maxX; yhigh = maxY; 

}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[])
{
for(int i = 0; i < num ; i ++ )
{

if(bufX[i] <= xhigh && bufX[i] >= xlow && bufY[i] <= yhigh && bufY[i] >= ylow )
{
// no need to multiply 0.001 as the numerator and denominator cancel out, 128 pixels 
// plot clear sets ymin as 32 and xmin as 0 .. 218 on both sides
	int32_t x = 128 * (bufX[i] - xlow) / (xhigh - xlow);   
	int32_t y = (128 * (yhigh - bufY[i]) / (yhigh - ylow)) + 32;   
ST7735_DrawPixel(x, y, ST7735_RED);
}	
  }
}

    