
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
// ** #1 **
// I have made a few minor changes in code that resulted
// in MAJOR behavioral changes in the viz. The best way
// to understand what it does, is to actually watch it do
// its thing for several minutes.
// ** #2 **
// This viz is an audio reactive, ever-changing 'lifeform'.
// If you get bored, the way to enforce a change in the
// pattern is to quiet down for a few minutes until the cube
// 'calms down' and then make a sound (whistle, clap, etc),
// and you will see a new random pattern emerge...
// ** #3 **
// ** HAVE PHUN **

#define GAIN_CONTROL 11
#define MAX_POINTS 20
#define SPEED 0.22
#define MIN_SALVO_SPACING 0
 
int threshhold;
int maximum=0;
int minimum=0;
float sensitivity=0;
int maxAmplitude=0;
bool aboveThreshhold=false;
int timeAboveThreshhold;
float newSpeed=SPEED;
int inputGain=63;
int maxBrightness=80;
int fadingMax=25;
int startAt=2;	//[0=base|1=center|2=top|3=random]

Cube cube = Cube();

/************************
 * type definitions     *
 * *********************/

typedef struct {
	Point raindrop;
  	float speed;
  	Color color;
  	bool flipped;
  	bool dead;
} raindrop;

typedef struct {
    raindrop raindrops[MAX_POINTS];
    bool dead;
} salvo;
 
salvo salvos[8];
 
 
void setup() {
  pinMode(7,OUTPUT);
  digitalWrite(7, HIGH);

  // Seed the random number generator.  THINGS WILL NEVER BE THE SAME AGAIN
  uint32_t seed = millis(); 
  srand(seed); 
  
  cube.begin();
  cube.show(); // Initialize all pixels to 'off'
  initMicrophone();
  initSalvos();
}
 
void loop() {
  	cube.background(black);
	checkMicrophone();
  	updateSalvos();
    drawSalvos();
  	cube.show();
	// Until particle fixes the bug that disconnects all photons 
   	// after flashing this statement will keep your cube connected.
  	delay(12);
}
 
void checkMicrophone() {
    int mic=analogRead(MICROPHONE);
    if(mic<minimum)
        minimum=mic;
    if(mic>maximum)
        maximum=mic;
    float range=maximum-minimum;
    int mean=(maximum-minimum)/2;
    /*
    if(minimum<mean)
        minimum++;
    if(maximum>mean)
        maximum--;
        */
    threshhold=mean+sensitivity*(range/2);
 
    if(mic>threshhold) {
        if((!aboveThreshhold)&&((timeAboveThreshhold-millis())>MIN_SALVO_SPACING)) {
            launchRain(mic-threshhold);
            aboveThreshhold=true;
            timeAboveThreshhold=millis();
        }
    }
    else
        aboveThreshhold=false;
/*
    Serial.print(mic);
    Serial.print(":  ");
    Serial.print(threshhold);
    Serial.print(" - above threshhold: ");
    Serial.println(aboveThreshhold);
    */
}
 
void launchRain(int amplitude) {
    int i;
    for(i=0;((i<cube.size)&&(!salvos[i].dead));i++)
        ;
    if(i<cube.size) {
        if(amplitude>maxAmplitude)
            maxAmplitude=amplitude;
        
      	int numDrops=map(amplitude,0, maxAmplitude,0, MAX_POINTS);
        for(int j=0;j<numDrops;j++) {
            salvos[i].dead=false;
          	salvos[i].raindrops[j].dead=false;
          	salvos[i].raindrops[j].flipped=false;
          	salvos[i].raindrops[j].speed=setNewSpeed();
            salvos[i].raindrops[j].raindrop.x=rand()%8;
            salvos[i].raindrops[j].raindrop.z=rand()%8;
			
          	switch(startAt) {
              case 0:	//base
          		salvos[i].raindrops[j].raindrop.y=((rand()%10)-5)/10;
                break;
              case 1:	//center
          		salvos[i].raindrops[j].raindrop.y=cube.size/2;
                break;
              case 2:	//top
          		salvos[i].raindrops[j].raindrop.y=cube.size;
                break;
              case 3:	//random
          		salvos[i].raindrops[j].raindrop.y=salvos[i].raindrops[j].speed>0 ? rand()%(cube.size/2) : (cube.size/2)+(rand()%(cube.size));
                break;
            }
          
         // Here's some cool combinations to try with cube.lerpColor():
         // purple, magenta
         // blue, pink
         // purple, pink
         // ..
         // ..
         // Go wild...
          	salvos[i].raindrops[j].color=cube.lerpColor(purple, magenta, j, SPEED, MAX_POINTS);
        }
      
        for(int j=numDrops;j<MAX_POINTS;j++) {
            salvos[i].raindrops[j].raindrop.x=-1;
            salvos[i].raindrops[j].raindrop.z=-1;
        }
    }
}

void drawSalvos() {
    for(int i=0;i<cube.size;i++)
        if(!salvos[i].dead)
            for(int j=0;j<MAX_POINTS;j++)
              	if(!salvos[i].raindrops[j].dead)
                	cube.setVoxel(salvos[i].raindrops[j].raindrop.x, salvos[i].raindrops[j].raindrop.y, salvos[i].raindrops[j].raindrop.z, salvos[i].raindrops[j].color);
}

void updateSalvos() {
    for(int i=0;i<cube.size;i++) {
        for(int j=0;j<MAX_POINTS;j++) {
            salvos[i].raindrops[j].raindrop.y+=salvos[i].raindrops[j].speed;
          	if(salvos[i].raindrops[j].speed>0) {
                if(salvos[i].raindrops[j].raindrop.y<cube.size) {
                  	if(salvos[i].raindrops[j].flipped)
                  		salvos[i].raindrops[j].color=cube.lerpColor(salvos[i].raindrops[j].color, black, i, abs(salvos[i].raindrops[j].speed), fadingMax);
                }
                else {
                  	if(salvos[i].raindrops[j].flipped)
                      	salvos[i].raindrops[j].dead=true;
                  	else {
                        salvos[i].raindrops[j].speed=-salvos[i].raindrops[j].speed;
                        salvos[i].raindrops[j].flipped=true;
                    }
                }
            }
          	else {
                if(salvos[i].raindrops[j].raindrop.y>0) {
                  	if(salvos[i].raindrops[j].flipped)
                  		salvos[i].raindrops[j].color=cube.lerpColor(salvos[i].raindrops[j].color, black, i, abs(salvos[i].raindrops[j].speed), fadingMax);
                }
                else {
                  	if(salvos[i].raindrops[j].flipped)
                      	salvos[i].raindrops[j].dead=true;
                  	else {
                      	salvos[i].raindrops[j].speed=-salvos[i].raindrops[j].speed;
                      	salvos[i].raindrops[j].flipped=true;
                    }
                }
            }
        }
      	
    	int offCube=true;
      	for(int j=0;j<MAX_POINTS;j++) {
          	if(!salvos[i].raindrops[j].dead) {
				offCube=false;
              	break;
            }
        }
      	if(offCube)
            salvos[i].dead=true;
    }
}
 
void initMicrophone() {
  pinMode(GAIN_CONTROL, OUTPUT);
  analogWrite(GAIN_CONTROL, inputGain);	//digitalWrite(GAIN_CONTROL, LOW);
}

float setNewSpeed() {
  	float ret;
    int rndSpeed=0+(rand()%7);
    switch(rndSpeed) {
      case 0:
        ret=0.5;
        break;
      case 1:
        ret=-0.5;
        break;
      case 2:
        ret=0.15;
        break;
      case 3:
        ret=-0.15;
        break;
      case 4:
        ret=0.25;
        break;
      case 5:
        ret=-0.25;
        break;
      case 6:
        ret=0.35;
        break;
      case 7:
        ret=-0.35;
        break;
      default:
        ret=SPEED;
        break;
    }
  	return ret;
}

void initSalvos() {
    for(int i=0;i<cube.size;i++) {
        for(int j=0;j<MAX_POINTS;j++) {
            salvos[i].raindrops[j].raindrop.x=-1;
            salvos[i].raindrops[j].raindrop.z=-1;
          	salvos[i].raindrops[j].speed=0;
          	salvos[i].raindrops[j].flipped=false;
          	salvos[i].raindrops[j].color=black;
          	salvos[i].raindrops[j].dead=true;
        }
        salvos[i].dead=true;
    }
}