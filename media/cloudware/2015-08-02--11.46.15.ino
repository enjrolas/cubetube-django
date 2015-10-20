
#include "beta-cube-library.h"

// Acid Rain
// Inspired by Kevin Darrah's "Rain",
// written for his 8x8x8 RGB cube.
//
// Modified to use the Neopixel Library by:
// Werner Moecke - 2015/08/01
//
// Code based on original work by Kevin Darrah:
// http://www.kevindarrah.com/?cat=99
//
// ** DISCLAIMER **
// ** DISCLAIMER **
// ** DISCLAIMER **
// THIS IS WORK IN PROGRESS - You are not in any obligation
// to load this viz in your cube.
// ** This viz was tested on a PHOTON - I have no idea if it 
//    will work the same way in different (older-generation) 
//    processors. If this is your case,
//             ** USE IT AT YOUR OWN RISK **
// ** END DISCLAIMER **
// ** END DISCLAIMER **
// ** END DISCLAIMER **
//
// ** NOTES **
// ** #1:
//   This viz uses a different approach to updating the
//   cube; during development, I decided not to use the 
//   Beta Cube Library, for the sake of maintaining the
//   compatibility with Kevin's original code.
// ** #2:
//   Because of the way the cube is updated, I struggled
//   attempting to run this viz while keeping the cube
//   connected.
//   THIS HAS BEEN RESOLVED - PHOTON NOW CONNECTS AT ALL TIMES
//
//   (Keeping the link below for historical purposes)
//	 Look in here for details on how to do a firmware reset: 
//	        http://docs.particle.io/photon/modes/
//
// ** HAVE PHUN **

#define REFRESH_CONTROL D1
#define SIDE 8

//****Accessing LEDs via NeoPixel directly this time****
Adafruit_NeoPixel strip=Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

//****Variables used by rainVersionTwo****
int x[64], y[64], z[64], addr, leds=64, ledcolor=0;
int xold[64], yold[64], zold[64];
//int xx[64], yy[64], zz[64];
//****Variables used by ISR****
Point base_coord[64];
Color base_color[64];
//****Variables used by LED****
Color voxel_color={0, 0, 0};
int wholebyte=0, blankingIndex=0, frameCounter=0;

//****Variables used to control the connection****
bool onlinePressed=false;
bool lastOnline=true;

//maxBrightness is the brightness limit for each pixel.  All color data will be scaled down 
//so that the largest value is maxBrightness
int maxBrightness=80;
int dimFactorBase=150;
int dimFactorDrop=300;

//****Function declarations****
void LED(int level, int row, int column, unsigned char red, unsigned char green, unsigned char blue);
void ISR();
//void show();
void initRain();
void rainVersionTwo();
void initCloudButton();
void checkCloudButton();
void background(Color col);
Color adjustBrightness(Color col, int amount_perc);


//****setup****
void setup(){
  	noInterrupts(); // Disable interrupts
  	pinMode(REFRESH_CONTROL, OUTPUT);
    digitalWrite(REFRESH_CONTROL, LOW);
	attachInterrupt(REFRESH_CONTROL, ISR, CHANGE);
  	tone(REFRESH_CONTROL, 1800, 0);	// Sets our refresh rate to 1.8KHz
  	initCloudButton();
  	interrupts();	// Start the square wave generator
  	
  	// Seed the random number generator.  THINGS WILL NEVER BE THE SAME AGAIN
    uint32_t seed = millis(); 
    randomSeed(seed); 

  	strip.begin();
  	initRain();	// Seed raindrops
	background(voxel_color); // Initialize all pixels to 'off'
  	strip.show();
}//***end setup***

void loop(){//***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop
	noInterrupts(); // Disable interrupts
    //if the 'connect to cloud' button is pressed, try to connect to wifi.  
    checkCloudButton();  	
    //otherwise, run the code
  	interrupts();	// Start the square wave generator
  	rainVersionTwo();
}//***end loop***

//sets up the online/offline switch
void initCloudButton(){
  //set the input mode for the 'connect to cloud' button
  pinMode(INTERNET_BUTTON, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);
    if(!digitalRead(MODE))  //if the wifi button is held down on boot, do a hard reset.  At any other time, keep the firmware, but try to add new wifi creds
    {
        WiFi.on();
        WiFi.clearCredentials();
        System.factoryReset();
    }
    //a.k.a. onlinePressed is HIGH when the switch is set to 'online' and LOW when the switch is set to 'offline'
    onlinePressed=!digitalRead(INTERNET_BUTTON);
    if(onlinePressed)
        Spark.connect();
}

//checks to see if the 'online/offline' switch is switched
void checkCloudButton(){
    //if the 'connect to cloud' button is pressed, try to connect to wifi.  
    //otherwise, run the program
    //note -- how does this behave when there are no wifi credentials loaded on the spark?

    //onlinePressed is HIGH when the switch is _not_ connected and LOW when the switch is connected
    //a.k.a. onlinePressed is HIGH when the switch is set to 'online' and LOW when the switch is set to 'offline'
    onlinePressed=!digitalRead(INTERNET_BUTTON);

    if((onlinePressed)&&(!lastOnline))  //marked as 'online'
    {
        lastOnline=onlinePressed;
        Spark.connect();
    }    

    else if((!onlinePressed)&&(lastOnline))  //marked as 'offline'
    {
        lastOnline=onlinePressed;
        Spark.disconnect();
    }

    lastOnline=onlinePressed;
    
    if(!digitalRead(MODE))
        WiFi.listen();
}

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
  
	wholebyte=(row*64)+(column*8)+level;
  	voxel_color={red, green, blue};
  	voxel_color=adjustBrightness(voxel_color, dimFactorDrop);
}//****LED routine end****

//This routine is called in the background automatically at frequency set by tone()
void ISR(){//***MultiPlex***
	// Blanking
  	/*for(addr=0; addr<leds; addr++){
      int blankingIndex=(xold[addr]*64)+(yold[addr]*8)+zold[addr];
      strip.setPixelColor(blankingIndex, strip.Color(0, 0, 0));
    }*/
  	Color faded={voxel_color.red-frameCounter, voxel_color.green-frameCounter, voxel_color.blue-frameCounter};
  	strip.setPixelColor(blankingIndex, strip.Color(faded.red, faded.green, faded.blue));
  	background(black);
	// Drawing drop
  	strip.setPixelColor(wholebyte, strip.Color(voxel_color.red, voxel_color.green, voxel_color.blue));
	// Drawing base
  	for(int addr=0; addr<leds; addr++){
      wholebyte=(base_coord[addr].z*64)+(base_coord[addr].y*8);	//+base_coord[addr].z;
      base_color[addr]=adjustBrightness(base_color[addr], dimFactorBase);
      strip.setPixelColor(wholebyte, strip.Color(base_color[addr].red, base_color[addr].green, base_color[addr].blue));
    }
  	strip.show();
  	frameCounter++;
}

void background(Color col){
    for(int x=0;x<SIDE;x++)
        for(int y=0;y<SIDE;y++)
            for(int z=0;z<SIDE;z++){
              	int index = (z*64) + (x*8) + y;
              	col=adjustBrightness(col, (dimFactorDrop+dimFactorBase)/2);
                strip.setPixelColor(index, strip.Color(col.red, col.green, col.blue));
            }
}

//Make changes to the cube visible.
//Causes pixel data to be written to the LED strips.
/*void show(){
  strip.show();
  if(onlinePressed){
	noInterrupts(); // Disable interrupts
    Spark.process();
    interrupts();	// Start the square wave generator
  }
}*/

//Adjust a color by setting each of its components
//to a percentage based on the current value setting
Color adjustBrightness(Color col, int amount_perc)
{
  	//set new components by a percentage of the actual values
  	col.red   = col.red*amount_perc/100;
  	col.green = col.green*amount_perc/100;
  	col.blue  = col.blue*amount_perc/100;
  	//constrain components to maxBrightness; might distort
  	//colors if the percentage was set too high
  	col.red   = constrain(col.red, 0, maxBrightness);
  	col.green = constrain(col.green, 0, maxBrightness);
  	col.blue  = constrain(col.blue, 0, maxBrightness);
  
  	return col;
}

//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE

void initRain(){
	for(addr=0; addr<64; addr++){
		x[addr]=random(8);
		y[addr]=random(8);
		z[addr]=random(8);
		//xx[addr]=random(16);
		//yy[addr]=random(16);
		//zz[addr]=random(16);     
	}
}

void rainVersionTwo(){//****rainVersionTwo****rainVersionTwo****rainVersionTwo****rainVersionTwo****rainVersionTwo
  	int slowdown;
    //unsigned long start;	//for a millis timer to cycle through the animations
  	//start=millis();
	//while(millis()-start<20000){
		if(ledcolor<200){
			for(addr=0; addr<leds; addr++){
              	//blankingIndex=(xold[addr]*64)+(yold[addr]*8)+zold[addr];
				LED(zold[addr], xold[addr], yold[addr], 0, 0, 0);
				if(z[addr]>=7){
                  	frameCounter=0;
					LED(z[addr], x[addr], y[addr], 0, 5, 15);
                }
				if(z[addr]==6){
					LED(z[addr], x[addr], y[addr], 0, 1, 9);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]==5){
					LED(z[addr], x[addr], y[addr], 0, 0, 10);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]==4){
					LED(z[addr], x[addr], y[addr], 1, 0, 11);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]==3){
					LED(z[addr], x[addr], y[addr], 3, 0, 12);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]==2){
					LED(z[addr], x[addr], y[addr], 10, 0, 15);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]==1){
					LED(z[addr], x[addr], y[addr], 10, 0, 10);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]<=0){
                  	base_color[addr]={10, 0, 1};
              		base_coord[addr]={z[addr], x[addr], y[addr]};
					LED(z[addr], x[addr], y[addr], 10, 0, 1);
              		//start=millis();
                }
			}
			slowdown=random(12, 23);
		}//200

		if(ledcolor>=200&&ledcolor<400){
			for(addr=0; addr<leds; addr++){
              	//blankingIndex=(xold[addr]*64)+(yold[addr]*8)+zold[addr];
				LED(zold[addr], xold[addr], yold[addr], 0, 0, 0);
				if(z[addr]>=7){
                  	frameCounter=0;
					LED(z[addr], x[addr], y[addr], 15, 15, 0);
                }
				if(z[addr]==6){
					LED(z[addr], x[addr], y[addr], 10, 10, 0);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
				if(z[addr]==5){
					LED(z[addr], x[addr], y[addr], 15, 5, 0);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
				if(z[addr]==4){
					LED(z[addr], x[addr], y[addr], 15, 2, 0); 
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]==3){
					LED(z[addr], x[addr], y[addr], 15, 1, 0);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]==2){
					LED(z[addr], x[addr], y[addr], 15, 0, 0);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]==1){
					LED(z[addr], x[addr], y[addr], 12, 0, 0);
                	blankingIndex=(x[addr]*64)+(y[addr]*8)+z[addr];
                }
              	if(z[addr]<=0){
                  	base_color[addr]={10, 0, 0};
              		base_coord[addr]={z[addr], x[addr], y[addr]};
					LED(z[addr], x[addr], y[addr], 10, 0, 0);
              		//start=millis();
                }
			}
			slowdown=random(12, 23);
		}//300
  
		ledcolor++;
		if(ledcolor>=400) ledcolor=0;

		for(addr=0; addr<leds; addr++){
			xold[addr]=x[addr];
			yold[addr]=y[addr];
			zold[addr]=z[addr];
		}
  
  		delay(slowdown);

  		for(addr=0; addr<leds; addr++){
			z[addr]--;	//z[addr] = z[addr]-1;

			if(z[addr]<random(-100,0)){
				x[addr]=random(8);
				y[addr]=random(8);
				
              	/*int select=random(3);
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
				}*/
				z[addr]=7; 
			}//-check
		}//add
	//}//while
}//rainv2