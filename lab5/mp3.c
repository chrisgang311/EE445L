 /** mp3.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 29th 2016
 * Description: Song metadata. contains all the song data
 * Lab: 5
 * TA: Dylan Zika
 * Date: September 29th 2016
 *********************************************************************************/
 #include "mp3.h"
 
//LEGEND OF ZELDA File Select
//melody
const unsigned long zelda_high[128] = {	
  
  A,D,BF0,G0,
  G,D,BF0,G0,
  GF,D,BF0,G0,
  G,D,BF0,G0,
    
  G,C0,A0,F0,
  F,C0,A0,F0,
  E,C0,A0,F0,
  F,C0,A0,F0,
  
  F,BF0,G0,E0,
  E,BF0,G0,E0,
  EF,BF0,G0,E0,
  E,BF0,G0,E0,

  E,A0,F0,D0,
  D,A0,F0,D0,
  DF,A0,F0,D0,
  D,A0,F0,D0,

  A,D,BF0,G0,
  G,D,BF0,G0,
  GF,D,BF0,G0,
  G,D,BF0,G0,
  
  BF,EF,C0,A0,
  A,EF,C0,A0,
  AF,EF,C0,A0,
  A,EF,C0,A0,

  C,D,BF0,G0,
  BF,D,BF0,G0,
  A,D,BF0,G0,
  BF,D,BF0,G0,
  
  A,BF0,G0,E0,
  G,BF0,G0,E0,
  F,BF0,G0,E0,
	E,BF0,G0,E0
	
};	

const unsigned long zelda_low[128] = {			
	
  BF7,BF7,D0,A0,
  A0,A0,A0,A0,
  A0,D0,A0,G0,
  C0,A0,G0,D0,
    
  A7,A7,C7,G0,
  G0,G0,G0,G0,
  G0,A7,G0,F0,
  C0,A0,G0,C7,
  
  G7,G7,C7,F0,
  F0,F0,F0,F0,
  F0,G7,F0,E0,
  G0,E0,C7,C7,

  F7,F7,A7,E0,
  E0,E0,E0,E0,
  F7,C7,E0,D0,
  G0,F0,C7,BF7,

  BF7,BF7,D0,A0,
  A0,A0,A0,A0,
  BF7,D0,A0,G0,
  D,A0,G0,D0,
  
  A7,A7,C7,C0,
  C0,C0,C0,C0,
  D0,D0,C0,A0,
  EF,D,C0,A0,

  G7,BF7,D0,BF0,
  BF0,BF0,BF0,BF0,
  G0,D0,BF0,A0,
  C0,BF0,G0,D0,
  
  C7,C7,G0,BF0,
  BF0,BF0,BF0,BF0,
  C7,C7,BF0,G0,
	E,D,G,E
	
};	


//poke center
// melody
const unsigned long pokemon_high[128] = {			
	
	D,A0,D,A,
	A,G,G,GF,
	E,DF,0,0,
	A0,A0,G0,A0,
		
	DF,A0,DF,GF,
	GF,E,E,DF,
	D,GF,0,0,
	D,DF,B0,A0,
	
	D,A0,D,A,
	A,G,G,GF,
	E,DF,0,0,
	A0,A0,G0,A0,
		
	DF,A0,DF,GF,
	GF,E,E,DF,
	D,D,D,0,
	GF0,G0,A0,B0,

	GF,GF,GF,GF,
	A,A,0,0,
	G,A,G,GF,
	E,E,0,0,
	
	DF,DF,DF,0,
	E,E,0,0,
	GF,G,GF,E,
	D,D,D,0,

	GF,GF,GF,0,
	A,A,0,0,
	G,GF,G,A,
	B,B,0,0,
	
	
	A,A,G,GF,
	G,G,0,0,
	GF,G,GF,E,
	D,D,0,0
	
};	

// MIDDLE
const unsigned long pokemon_middle[128] = {			
	
	GF0,F0,GF0,D,
	D,DF,B0,A0,
	B0,A0,G0,GF0,
	E0,GF0,E0,0,
		
	A0,E0,A0,DF,
	DF,B0,A0,G0,
	GF0,A0,B0,DF,
	A0,0,E0,0,
	
	GF0,F0,GF0,D,
	D,DF,B0,A0,
	B0,A0,G0,GF0,
	E0,GF0,E0,0,
		
	A0,E0,A0,DF,
	DF,B0,A0,G0,
	GF0,E0,D0,E0,
	D0,0,E0,0,

	GF0,E0,D0,0,
	E0,GF0,G0,A0,
	B0,A0,G0,0,
	E0,GF0,G0,A0,
	
	G0,GF0,E0,0,
	DF0,D0,E0,G0,
	GF0,G0,A0,B0,
	A0,A0,A0,0,

	D,DF,B0,0,
	A0,B0,DF,D,
	E,D,DF,0,
	B0,DF,D,E,
	
	
	DF,B0,A0,0,
	G0,A0,B0,G0,
	A0,G0,GF0,E0,
	D0,E0,GF0,G0
	
};	

// LOW
const unsigned long pokemon_low[128] = {			
	
	D7,GF7,D7,GF7,
	D7,GF7,G7,GF7,
	E7,A7,E7,A7,
	E7,A7,E7,A7,
		
	E7,A7,E7,A7,
	E7,A7,G7,A7,
	GF7,A7,GF7,A7,
	GF7,A7,G7,A7,
	
	D7,GF7,D7,GF7,
	D7,GF7,G7,GF7,
	E7,A7,E7,A7,
	E7,A7,E7,A7,
		
	E7,A7,E7,A7,
	E7,A7,G7,A7,
	D7,GF7,D7,GF7,
	D7,A7,G7,GF7,

	GF7,A7,GF7,A7,
	GF7,A7,GF7,A7,
	G7,BF7,G7,BF7,
	G7,BF7,G7,BF7,
	
	E7,G7,E7,G7,
	E7,G7,E7,G7,
	GF7,A7,GF7,A7,
	GF7,A7,GF7,A7,

	GF7,A7,GF7,A7,
	GF7,A7,GF7,A7,
	G7,BF7,G7,BF7,
	G7,BF7,G7,BF7,
	
	E7,G7,E7,G7,
	E7,G7,E7,G7,
	GF7,A7,GF7,A7,
	GF7,A7,GF7,A7,
	
};	

/* melody */
const unsigned long int cerulean_high[288] = {      
  
	/* intro */
	E,0,EF,0,
  DF,0,B0,0,
  A0,0,B0,0,
  DF,0,EF,0,
  
  E,E,0,0,
  0,0,E,B0,
  DF,DF,EF,EF,
	E,GF,AF,A,
	
	/* beginning */
  AF,AF,AF,AF,
  AF,AF,A,AF,
  GF,GF,GF,GF,
  GF,GF,0,0,
    
  0,0,0,0,
  0,0,E,B0,
  DF,DF,EF,EF,
  E,GF,AF,A,
  
  AF,AF,AF,AF,
  AF,AF,E,AF,
  B,B,B,B,
  B,B,0,0,
    
  0,0,0,0,
  0,0,E,B0,
  DF,DF,EF,EF,
  E,GF,AF,A,

  AF,AF,AF,AF,
  AF,AF,A,AF,
  GF,GF,GF,GF,
  GF,GF,0,0,
  
  AF0,0,AF0,0,
  AF0,0,E,B0,
  DF,DF,EF,EF,
  E,GF,AF,A,

  AF,AF,AF,AF,
  AF,AF,E,AF,
  B,B,B,B,
  B,B,0,0,
  
  /*half*/
  DF,B0,DF,EF,
  E,E,0,0,
  AF,AF,0,0,
  E,E,0,0,  
  
  EF,DF,EF,E,
  GF,GF,0,0,
  B,B,0,0,
  GF,GF,0,0,
    
  DF,B0,DF,EF,
  E,0,E,EF,
  E,GF,AF,0,
  AF,A,AF,A,
  
  B,GF,EF,DF,
  B0,DF,EF,GF,
  B,B,B,B,/* END ON EITGH OR QUARTER? */
  0,0,0,0,
    
  E0,E0,E0,E0,
  E0,E0,E0,0,
  B7,B7,B7,0,
  GF0,GF0,GF0,0,

  AF0,AF0,AF0,0,
  A0,A0,A0,0,
  B0,B0,B0,B0,
  B0,B0,B0,0,
  
  B0,B0,B0,B0,
  B0,B0,B0,0,
  AF0,AF0,AF0,0,
  EF,EF,EF,0,

  DF,DF,DF,0,
  EF,EF,EF,0,
  E,0,EF,0,
  DF,0,EF,0,
  
  E,0,0,0,
  0,0,E,B0,
  DF,DF,EF,EF,
  E,GF,AF,A,
  
};
  
/* harmony */
const unsigned long int cerulean_middle[288] = {   

	/* intro */
	0,0,0,0,
  0,0,0,0,
  E0,0,EF0,0,
  DF0,0,C7,0,
  
  B7,B7,0,0,
  0,0,0,0,
  0,0,0,0,
	0,0,0,0,
	
	/* beginning */
  B0,B0,B0,B0,
  DF,DF,EF,D,
  DF,DF,0,0,
  B0,B0,0,0,
    
  AF0,AF0,AF0,AF0,
  AF0,AF0,AF0,0,
  E,E,E,E,
  E,E,E,0,
  
  A0,A0,A0,0,
  AF0,0,GF0,0,
  AF0,0,A0,0,
  B0,0,DF,0,
    
  0,0,0,0,
  0,0,E,B0,
  DF,DF,EF,EF,
  E,GF,AF,A,

  DF,DF,DF,0,
  B0,B0,DF,EF,
  DF,DF,DF,0,
  B,B,B,0,
  
  0,E0,0,E0,
  0,0,0,0,
  AF0,AF0,AF0,AF0,
  AF0,AF0,AF0,0,

  B0,B0,B0,0,
  A0,0,AF0,0,
  GF0,0,AF0,0,
  A0,0,B0,0,
  
  /*half*/
  
  E0,0,GF0,0,
  AF0,0,0,0,
  E,E,0,0,
  B0,B0,0,0,  
  
  GF0,0,AF0,0,
  A0,0,0,0,
  GF,GF,0,0,
  EF,EF,0,0,  
  
  E0,0,0,0,
  0,0,GF0,0,
  0,0,0,0,
  A0,0,0,0,
  
  B0,0,0,0,
  0,0,0,0,
  0,0,0,0,/* END ON EITGH OR QUARTER? */
  0,0,0,0,
    
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,

  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  
  E0,E0,E0,E0,
  E0,E0,E0,0,
  0,0,0,0,
  0,0,0,0,

  GF0,GF0,GF0,GF0,
  GF0,0,GF0,0,
  AF0,0,AF0,0,
  GF0,0,GF0,0,
  
  E0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  
};

/* bass */
const unsigned long int cerulean_low[288] = {      
  
	/* intro */
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  
  E7,E7,AF7,AF7,
  E7,E7,AF7,AF7,
  E7,E7,AF7,AF7,
	E7,E7,AF7,AF7,
	
	/* beginning */
  GF7,0,A7,0,
  GF7,0,A7,0,
  B7,0,A7,0,
  AF7,0,GF7,0,
    
  E7,0,AF7,0,
  E7,0,AF7,0,
  E7,0,AF7,0,
  E7,0,AF7,0,
  
  GF7,0,A7,0,
  GF7,0,A7,0,
  B7,0,A7,0,
  AF7,0,GF7,0,
    
  E7,0,AF7,0,
  E7,0,AF7,0,
  E7,0,AF7,0,
  E7,0,AF7,0,
  
  GF7,0,A7,0,
  GF7,0,A7,0,
  B7,0,A7,0,
  AF7,0,GF7,0,
    
  E7,0,AF7,0,
  E7,0,AF7,0,
  E7,0,AF7,0,
  E7,0,AF7,0,
  
  GF7,0,A7,0,
  GF7,0,A7,0,
  B7,0,A7,0,
  AF7,0,GF7,0,
    
  /*half*/
  
  DF7,0,E7,0,
  DF7,0,0,0,
  AF7,0,0,0,
  E7,0,0,0,
    
  EF7,0,GF7,0,
  EF7,0,0,0,
  B7,0,0,0,
  GF7,0,0,0,
  
    
  DF7,0,0,0,
  0,0,E7,0,
  0,0,0,0,
  AF7,0,A7,0,
  
  B7,0,0,0,
  0,0,0,0,
  0,0,A7,0,
  AF7,0,GF7,0,
  
  E7,0,AF7,0,
  E7,0,AF7,0,
  E7,0,AF7,0,
  E7,0,AF7,0,
  
  GF7,0,A7,0,
  GF7,0,A7,0,
  B7,0,A7,0,
  AF7,0,GF7,0,

  AF7,0,B7,0,
  AF7,0,B7,0,
  AF7,0,B7,0,
  AF7,0,B7,0,
  
  GF7,0,A7,0,
  GF7,0,A7,0,
  B7,0,A7,0,
  AF7,0,GF7,0

};


/** instruments **/
const unsigned short Wave[32] = {  
  2048,2438,2813,3159,3462,3711,3896,4010,4048,4010,3896,
  3711,3462,3159,2813,2438,2048,1658,1283,937,634,385,
  200,86,48,86,200,385,634,937,1283,1658
};  
const unsigned short Flute[32] = {  
  1007,1252,1374,1548,1698,1797,1825,1797,1675,1562,1383,
  1219,1092,1007,913,890,833,847,810,777,744,674,
  598,551,509,476,495,533,589,659,758,876
};  
const unsigned short Trumpet[32] = {  
  2013,2179,2318,2397,2135,1567,572,153,1567,3043,2973,
  2353,2135,2074,2170,2126,2022,2030,2091,2126,2022,2022,
  2118,2231,2170,2153,2161,2214,2179,2100,2030,2091
};  


 //LEGEND OF ZELDA File Select
const mp3 ZELDA = {128,  1.2 * eigth_notes, "zelda theme", {Wave, Flute, Wave}, zelda_high, zelda_high, zelda_high};

 //Pokemon Center
const mp3 POKEMON = {128, quarter_notes, "pokemon center", {Wave, Flute, Wave}, pokemon_high, pokemon_middle, pokemon_low};

 //Pokemon Center
const mp3 CERULEAN = {288, 0.75 * eigth_notes, "cerulean city", {Flute, Flute, Wave}, cerulean_high, cerulean_middle, cerulean_low};
