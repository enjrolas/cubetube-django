
#include "beta-cube-library.h"

// Purple Rain
// L3D Cube Guys (a.k.a. Looking Glass) - 2014/??/??
//
// Modified to use the Beta Cube Library by:
// Werner Moecke - 2015/07/24
//
// Code lifted from original L3D demo on old.cubetube
//
// ** DISCLAIMER **
// THIS IS WORK IN PROGRESS
//
// ** HINTS **
// ** #1:
//   I have made a few minor changes in code that resulted
//   in MAJOR behavioral changes in the viz. The best way
//   to understand what it does, is to actually watch it do
//   its thing for several minutes.
// ** #2:
//   This viz is an audio reactive, ever-changing 'lifeform'.
//   If you get bored, the way to enforce a change in the
//   pattern is to quiet down for a few seconds until the cube
//   'calms down' and then make a sound (whistle, clap, etc),
//   and you will see a new random pattern emerge...
// ** #3:
//   Raindrops can now start at a definite position within the
//   y-axis. There's even a variable to allow control over it!
// ** HAVE PHUN **

#define REFRESH_CONTROL D1
#define SIDE 8

//****Accessing LEDs via NeoPixel directly this time****
Adafruit_NeoPixel strip=Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);	//Cube cube = Cube();

//****These variables can be used for other things****
int x[64], y[64], z[64], addr, leds=64, ledcolor=0;
int xx[64], yy[64], zz[64], xold[64], yold[64], zold[64];
int wholebyte=0;
Point base_coord[64];
Color base_color[64];
Color voxel_color={0, 0, 0};
bool redrawBase=true;

//****Function declarations****
void LED(int level, int row, int column, unsigned char red, unsigned char green, unsigned char blue);
void ISR();
void background(Color col);
void rainVersionTwo();


//****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup
void setup(){
  	//Serial.begin(115200);// if you need it?
  	noInterrupts(); // disable interrupts

  	pinMode(REFRESH_CONTROL, OUTPUT);
    digitalWrite(REFRESH_CONTROL, LOW);
	attachInterrupt(REFRESH_CONTROL, ISR, CHANGE);
  	tone(REFRESH_CONTROL, 1000, 0);
	
  	interrupts();	// re-enable interrupts
  	
  	// Seed the random number generator.  THINGS WILL NEVER BE THE SAME AGAIN
    uint32_t seed = millis(); 
    randomSeed(seed); 

	for(addr=0; addr<64; addr++){
		x[addr]=random(8);
		y[addr]=random(8);
		z[addr]=random(8);
		xx[addr]=random(16);
		yy[addr]=random(16);
		zz[addr]=random(16);     
	}
	
  	strip.begin();
	background(voxel_color); // Initialize all pixels to 'off'
  	strip.show();
}//***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup

void loop(){//***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop
  	rainVersionTwo();
}//***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop


//This is where it all starts
//This routine is how LEDs are updated, with the inputs for the LED location and its R G and B brightness levels
// ****To control an LED, you simply:
// LED(level you want 0-7, row you want 0-7, column you want 0-7, red brighness 0-15, green brighness 0-15, blue brighness 0-15);
void LED(int level, int row, int column, unsigned char red, unsigned char green, unsigned char blue){ //****LED Routine****LED Routine****LED Routine****LED Routine
	// First, check and make sure nothing went beyond the limits, just clamp things at either 0 or 7 for location, and 0 or 15 for brightness
	level=constrain(level, 0, 7);
	row=constrain(row, 0, 7);
	column=constrain(column, 0, 7);
	red=constrain(red, 0, 15);
	green=constrain(green, 0, 15);
	blue=constrain(blue, 0, 15);
  
	// This next variable is the same thing as before, but here we don't divide by 8, so we get the LED number 0-511
	wholebyte=(row*64)+(column*8)+level;
  	voxel_color.red=red;
  	voxel_color.green=green;
  	voxel_color.blue=blue;
}//****LED routine end****LED routine end****LED routine end****LED routine end****LED routine end****LED routine end****LED routine end

void ISR(){//***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM
	//This routine is called in the background automatically at frequency set by OCR1A
	//In this code, I set OCR1A to 30, so this is called every 124us, giving each level in the cube 124us of ON time
	//There are 8 levels, so we have a maximum brightness of 1/8, since the level must turn off before the next level is turned on
	//The frequency of the multiplexing is then 124us*8=992us, or 1/992us= about 1kHz
	background(black);
    strip.setPixelColor(wholebyte, strip.Color(voxel_color.red, voxel_color.green, voxel_color.blue));
    for(int addr=0; addr<leds; addr++){
      	wholebyte=(base_coord[addr].x*64)+(base_coord[addr].y*8)+base_coord[addr].x;
    	strip.setPixelColor(wholebyte, strip.Color(base_color[addr].red, base_color[addr].green, base_color[addr].blue));
		//LED(base_coord[addr].x, base_coord[addr].y, base_coord[addr].z, base_color[addr].red, base_color[addr].green, base_color[addr].blue);	//redrawBase=true;
    }
  	/*if(redrawBase)
      for(addr=0; addr<leds; addr++){
        wholebyte=(base_coord[addr].z*64)+(base_coord[addr].y*8);	//+base_coord[addr].z;
    	strip.setPixelColor(wholebyte, strip.Color(base_color[addr].red, base_color[addr].green, base_color[addr].blue));
      }*/
  	strip.show();
	delay(5);
}

void background(Color col)
{
    for(int x=0;x<SIDE;x++)
        for(int y=0;y<SIDE;y++)
            for(int z=0;z<SIDE;z++){
              	int index = (z*64) + (x*8) + y;
                strip.setPixelColor(index, strip.Color(col.red, col.green, col.blue));
            }
}


//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE

void rainVersionTwo(){//****rainVersionTwo****rainVersionTwo****rainVersionTwo****rainVersionTwo****rainVersionTwo
    unsigned long start;	//for a millis timer to cycle through the animations
  	
  	//start=millis();
	//while(millis()-start<20000){
		if(ledcolor<200){
			for(addr=0; addr<leds; addr++){
				LED(zold[addr], xold[addr], yold[addr], 0, 0, 0);
				if(z[addr]>=7)
					LED(z[addr], x[addr], y[addr], 0, 5, 15);
				if(z[addr]==6)
					LED(z[addr], x[addr], y[addr], 0, 1, 9);
				if(z[addr]==5)
					LED(z[addr], x[addr], y[addr], 0, 0, 10);
				if(z[addr]==4)
					LED(z[addr], x[addr], y[addr], 1, 0, 11); 
				if(z[addr]==3)
					LED(z[addr], x[addr], y[addr], 3, 0, 12);
				if(z[addr]==2)
					LED(z[addr], x[addr], y[addr], 10, 0, 15);
				if(z[addr]==1)
					LED(z[addr], x[addr], y[addr], 10, 0, 10);
				if(z[addr]<=0){
                  	base_color[addr]={10, 0, 1};
              		base_coord[addr]={z[addr], x[addr], y[addr]};
					LED(z[addr], x[addr], y[addr], 10, 0, 1);
              		start=millis();
                }
			}
		}//200

		if(ledcolor>=200&&ledcolor<400){
			for(addr=0; addr<leds; addr++){
				LED(zold[addr], xold[addr], yold[addr], 0, 0, 0);
				if(z[addr]>=7)
					LED(z[addr], x[addr], y[addr], 15, 15, 0);
				if(z[addr]==6)
					LED(z[addr], x[addr], y[addr], 10, 10, 0);
				if(z[addr]==5)
					LED(z[addr], x[addr], y[addr], 15, 5, 0);
				if(z[addr]==4)
					LED(z[addr], x[addr], y[addr], 15, 2, 0); 
				if(z[addr]==3)
					LED(z[addr], x[addr], y[addr], 15, 1, 0);
				if(z[addr]==2)
					LED(z[addr], x[addr], y[addr], 15, 0, 0);
				if(z[addr]==1)
					LED(z[addr], x[addr], y[addr], 12, 0, 0);
				if(z[addr]<=0){
                  	base_color[addr]={10, 0, 0};
              		base_coord[addr]={z[addr], x[addr], y[addr]};
					LED(z[addr], x[addr], y[addr], 10, 0, 0);
              		start=millis();
                }
			}
		}//300

		if(ledcolor>=300&&ledcolor<400){

		}
		if(ledcolor>=500&&ledcolor<600){

		}
  
		ledcolor++;
		if(ledcolor>=400) ledcolor=0;

		for(addr=0; addr<leds; addr++){
			xold[addr]=x[addr];
			yold[addr]=y[addr];
			zold[addr]=z[addr];
		} 
      	
		redrawBase=(millis()-start<20000);
  
  		delay(15);

  		for(addr=0; addr<leds; addr++){
			z[addr]--;	//z[addr] = z[addr]-1;

			if(z[addr]<random(-100,0)){
				x[addr]=random(8);
				y[addr]=random(8);
				
              	int select=random(3);
				if(select==0){
					xx[addr]=0;
					zz[addr]=random(16);
					yy[addr]=random(16);
				}
				if(select==1){
					xx[addr]=random(16);
					zz[addr]=0;
					yy[addr]=random(16);
				}
				if(select==2){
					xx[addr]=random(16);
					zz[addr]=random(16);
					yy[addr]=0;
				}    
				z[addr]=7; 
			}//-check
		}//add
	//}//while
}//rainv2