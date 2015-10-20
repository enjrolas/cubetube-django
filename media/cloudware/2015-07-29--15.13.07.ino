
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

Cube cube = Cube();

//This is how the brightness for every LED is stored,  
//Each LED only needs a 'bit' to know if it should be ON or OFF, so 64 Bytes gives you 512 bits= 512 LEDs
//Since we are modulating the LEDs, using 4 bit resolution, each color has 4 arrays containing 64 bits each
//byte red0[64], red1[64], red2[64], red3[64];
//byte blue0[64], blue1[64], blue2[64], blue3[64];
//byte green0[64], green1[64], green2[64], green3[64];
//notice how more resolution will eat up more of your precious RAM

//int level=0;//keeps track of which level we are shifting data to
//int anodelevel=0;//this increments through the anode levels

//These variables can be used for other things
unsigned long start;//for a millis timer to cycle through the animations

//****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup
void setup(){
  	//Serial.begin(115200);// if you need it?
	//noInterrupts();// kill interrupts until everybody is set up

    pinMode(7,OUTPUT);
    digitalWrite(7, HIGH);

    // Seed the random number generator.  THINGS WILL NEVER BE THE SAME AGAIN
    uint32_t seed = millis(); 
    srand(seed); 

    cube.begin();
    cube.show(); // Initialize all pixels to 'off'

	// here I just set up the anode array, this is what's written to the anode shift register, to enable each level
	//anode[0]=B00000001;
	//anode[1]=B00000010;
	//anode[2]=B00000100;
	//anode[3]=B00001000;
	//anode[4]=B00010000;
	//anode[5]=B00100000;
	//anode[6]=B01000000;
	//anode[7]=B10000000;
	// don't hate on how I assigned the values to this register! haha

	//interrupts();//let the show begin, this lets the multiplexing start
}//***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup

void loop(){//***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop
	//cube.background(black);
  	rainVersionTwo();
  	cube.show();

}//***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop

void rainVersionTwo(){//****rainVersionTwo****rainVersionTwo****rainVersionTwo****rainVersionTwo****rainVersionTwo
	int x[64], y[64], z[64], addr, leds=64, bright=1, ledcolor, colowheel;
	int xx[64], yy[64], zz[64], xold[64], yold[64], zold[64], slowdown;

	for(addr=0; addr<64; addr++){
		x[addr]=random(8);
		y[addr]=random(8);
		z[addr]=random(8);
		xx[addr]=random(16);
		yy[addr]=random(16);
		zz[addr]=random(16);     
	}
	
	start=millis();
	while(millis()-start<800){	//20000){
		//wipe_out();
		//for(addr=0; addr<leds; addr++)
		//LED(zold[addr], xold[addr], yold[addr], 0, 0, 0);

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
				if(z[addr]<=0)
					LED(z[addr], x[addr], y[addr], 10, 0, 1);
			}
		}//200

		if(ledcolor>=200&&ledcolor<300){
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
				if(z[addr]<=0)
					LED(z[addr], x[addr], y[addr], 10, 0, 0);
			}
		}//300

		if(ledcolor>=300&&ledcolor<400){

		}
		if(ledcolor>=500&&ledcolor<600){

		}

		ledcolor++;
		if(ledcolor>=300) ledcolor=0;

		for(addr=0; addr<leds; addr++){
			xold[addr]=x[addr];
			yold[addr]=y[addr];
			zold[addr]=z[addr];
		} 

		delay(15);

		//for(addr=0; addr<leds; addr++)
		//LED(z[addr], x[addr], y[addr], 0, 0, 0);
		for(addr=0; addr<leds; addr++){
			//slowdown = random(2);
			//if(bitRead(z[addr],0))
			z[addr] = z[addr]-1;

			// x[addr] = x[addr]+1;
			// y[addr] = y[addr]+1;
			if(z[addr]<random(-100,0)){
				x[addr]=random(8);
				y[addr]=random(8);
				
              	int select=random(3);
				if(select==0){
					xx[addr]=0;
					zz[addr]=random(16);
					yy[addr]=random(16);
					//zz[addr]=0;
				}
				if(select==1){
					xx[addr]=random(16);
					zz[addr]=0;
					yy[addr]=random(16);
					//yy[addr]=0;
				}
				if(select==2){
					xx[addr]=random(16);
					zz[addr]=random(16);
					yy[addr]=0;
				}    
				z[addr]=7; 
			}//-check
		}//add
	}//while
}//rainv2


//This is where it all starts
//This routine is how LEDs are updated, with the inputs for the LED location and its R G and B brightness levels
// To control an LED, you simply:
// LED(level you want 0-7, row you want 0-7, column you want 0-7, red brighness 0-15, green brighness 0-15, blue brighness 0-15);
void LED(int level, int row, int column, unsigned char red, unsigned char green, unsigned char blue){ //****LED Routine****LED Routine****LED Routine****LED Routine
	// First, check and make sure nothing went beyond the limits, just clamp things at either 0 or 7 for location, and 0 or 15 for brightness
	if(level<0)  level=0;
	if(level>7)	 level=7;
	if(row<0)    row=0;
	if(row>7)    row=7;
	if(column<0) column=0;
	if(column>7) column=7;  
	if(red<0) 	 red=0;
	if(red>15) 	 red=15;
	if(green<0)  green=0;
	if(green>15) green=15;
	if(blue<0) 	 blue=0;
	if(blue>15)  blue=15;  

  	Color col={red, green, blue};
  	//cube.setVoxel(row,    column, level, col);
  	//cube.setVoxel(level,  row,	  column, col);
  	//Z*cube.setVoxel(column, level,  row, col);
  	//cube.setVoxel(row,	  column, level, col);
  	//cube.setVoxel(level,  row,	  column, col);
  	cube.setVoxel(column, level,  row, col);
	//There are 512 LEDs in the cube, so when we write to level 2, column 5, row 4, that needs to be translated into a number from 0 to 511

	//This looks confusing, I know...
	//int whichbyte = int(((level*64)+(row*8)+column)/8);

	// The first level LEDs are first in the sequence, then 2nd level, then third, and so on
	//the (level*64) is what indexes the level's starting place, so level 0 are LEDs 0-63, level 1 are LEDs 64-127, and so on

	//The column counts left to right 0-7 and the row is back to front 0-7
	//This means that if you had level 0, row 0, the bottom back row would count from 0-7, 

	//so if you looked down on the cube, and only looked at the bottom level
	// 00 01 02 03 04 05 06 07
	// 08 09 10 11 12 13 14 15
	// 16 17 18 19 20 21 22 23
	// 24 25 26 27 28 29 30 31
	// 32 33 34 35 36 37 38 39
	// 40 41 42 43 44 45 46 47  
	// 48 49 50 51 52 53 54 55  
	// 56 57 58 59 60 61 62 63

	//Then, if you incremented the level, the top right of the grid above would start at 64
	//The reason for doing this, is so you don't have to memorize a number for each LED, allowing you to use level, row, column

	//Now, what about the divide by 8 in there?
	//...well, we have 8 bits per byte, and we have 64 bytes in memory for all 512 bits needed for each LED, so
	//we divide the number we just found by 8, and take the integ7er of it, so we know which byte, that bit is located
	//confused? that's ok, let's take an example, if we wanted to write to the LED to the last LED in the cube, we would write a 7, 7, 7
	// giving (7*64)+(7*8)=7 = 511, which is right, but now let's divide it by 8, 511/8 = 63.875, and take the int of it so, we get 63,
	//this is the last byte in the array, which is right since this is the last LED

	// This next variable is the same thing as before, but here we don't divide by 8, so we get the LED number 0-511
	//int wholebyte=(level*64)+(row*8)+column;
	//This will all make sense in a sec

	//This is 4 bit color resolution, so each color contains x4 64 byte arrays, explanation below:
	//bitWrite(red0[whichbyte], wholebyte-(8*whichbyte), bitRead(red, 0));
	//bitWrite(red1[whichbyte], wholebyte-(8*whichbyte), bitRead(red, 1));
	//bitWrite(red2[whichbyte], wholebyte-(8*whichbyte), bitRead(red, 2)); 
	//bitWrite(red3[whichbyte], wholebyte-(8*whichbyte), bitRead(red, 3)); 
	//
	//bitWrite(green0[whichbyte], wholebyte-(8*whichbyte), bitRead(green, 0));
	//bitWrite(green1[whichbyte], wholebyte-(8*whichbyte), bitRead(green, 1));
	//bitWrite(green2[whichbyte], wholebyte-(8*whichbyte), bitRead(green, 2)); 
	//bitWrite(green3[whichbyte], wholebyte-(8*whichbyte), bitRead(green, 3));
	//
	//bitWrite(blue0[whichbyte], wholebyte-(8*whichbyte), bitRead(blue, 0));
	//bitWrite(blue1[whichbyte], wholebyte-(8*whichbyte), bitRead(blue, 1));
	//bitWrite(blue2[whichbyte], wholebyte-(8*whichbyte), bitRead(blue, 2)); 
	//bitWrite(blue3[whichbyte], wholebyte-(8*whichbyte), bitRead(blue, 3));

	//Are you now more confused?  You shouldn't be!  It's starting to make sense now.  Notice how each line is a bitWrite, which is,
	//bitWrite(the byte you want to write to, the bit of the byte to write, and the 0 or 1 you want to write)
	//This means that the 'whichbyte' is the byte from 0-63 in which the bit corresponding to the LED from 0-511
	//Is making sense now why we did that? taking a value from 0-511 and converting it to a value from 0-63, since each LED represents a bit in 
	//an array of 64 bytes.
	//Then next line is which bit 'wholebyte-(8*whichbyte)'  
	//This is simply taking the LED's value of 0-511 and subracting it from the BYTE its bit was located in times 8
	//Think about it, byte 63 will contain LEDs from 504 to 511, so if you took 505-(8*63), you get a 1, meaning that,
	//LED number 505 is is located in bit 1 of byte 63 in the array

	//is that it?  No, you still have to do the bitRead of the brightness 0-15 you are trying to write,
	//if you wrote a 15 to RED, all 4 arrays for that LED would have a 1 for that bit, meaning it will be on 100%
	//This is why the four arrays read 0-4 of the value entered in for RED, GREEN, and BLUE
	//hopefully this all makes some sense?
}//****LED routine end****LED routine end****LED routine end****LED routine end****LED routine end****LED routine end****LED routine end