
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
// ** 1 **
// I have made a few minor changes in code that resulted
// in MAJOR behavioral changes in the viz. The best way
// to understand what it does, is to actually watch it do
// its thing for several minutes.
// ** 2 **
// This viz is an audio reactive, ever-changing 'lifeform'.
// If you got bored, the way to enforce a change in the
// pattern is to quiet down for a few minutes until the cube
// 'calms down' and then make a sound (whistle, clap, etc),
// and you will see a new random pattern emerge...
// ** 3 **
// If you just want the 'old raindrop' visuals back, read
// the comments just above the updateSalvos() function.
//
// ** HAVE PHUN **

#define GAIN_CONTROL 11
#define MAX_POINTS 15
#define SPEED 0.22
#define MIN_SALVO_SPACING 100
 
int threshhold;
int maximum;
int minimum;
float sensitivity=0.5;
int maxAmplitude=0;
bool aboveThreshhold=false;
int timeAboveThreshhold;
float newSpeed=SPEED;

int maxBrightness=50;

Cube cube = Cube();

/************************
 * type definitions     *
 * *********************/
 
typedef struct{
    Point raindrops[MAX_POINTS];
    bool dead;
} salvo;
 
Color rainColor;
salvo salvos[8];
 
 
void setup() {
  pinMode(7,OUTPUT);
  digitalWrite(7, HIGH);
  // seed the random number generator.  THINGS WILL NEVER BE THE SAME AGAIN
  uint32_t seed = millis(); 
  srand(seed); 
  
  cube.begin();
  cube.show(); // Initialize all pixels to 'off'
  //Serial.begin(115200);
  //Serial.println("initializing...");
  initMicrophone();
  initSalvos();
  //rainColor.red=100;
  //rainColor.green=0;//80;
  //rainColor.blue=100;
}
 
void loop() {
  	cube.background(black);
	checkMicrophone();
  	updateSalvos();
    drawSalvos();
  	cube.show();
	
   // had to introduce this delay, because the 
   // cube keeps disconnecting - unfortunately,
   // this causes the animation to underperform...
   // Until this issue is corrected on cubetube,
   // this statement will keep your cube connected.
  	delay(15);
}
 
void checkMicrophone()
{
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
 
    if(mic>threshhold)
    {
        if((!aboveThreshhold)&&((timeAboveThreshhold-millis())>MIN_SALVO_SPACING))
        {
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
 
void launchRain(int amplitude)
{
    int i;
    for(i=0;((i<cube.size)&&(!salvos[i].dead));i++)
        ;
    if(i<cube.size)
    {
        if(amplitude>maxAmplitude)
            maxAmplitude=amplitude;
        int numDrops=map(amplitude,0, maxAmplitude,0, MAX_POINTS);
        for(int j=0;j<numDrops;j++)
        {
            salvos[i].raindrops[j].x=rand()%8;
            salvos[i].raindrops[j].y=((rand()%10)-5)/10;
            salvos[i].raindrops[j].z=rand()%8;
            salvos[i].dead=false;
        }
        for(int j=numDrops;j<MAX_POINTS;j++)
        {
            salvos[i].raindrops[j].x=-1;
            salvos[i].raindrops[j].z=-1;
        }
 
    }
   
}

 // Here's some cool combinations to try with cube.lerpColor():
 // purple, magenta
 // blue, pink
 // purple, pink
 // ..
 // ..
 // Go wild...
void drawSalvos()
{
    for(int i=0;i<cube.size;i++)
        if(!salvos[i].dead)
            for(int j=0;j<MAX_POINTS;j++)
                cube.setVoxel(salvos[i].raindrops[j].x, salvos[i].raindrops[j].y, salvos[i].raindrops[j].z, cube.lerpColor(purple, magenta, j, SPEED, MAX_POINTS)); //rainColor);
}

// OK, if you found this "raining up'n'down" thing became
// too crazy for you and you want to "bring rain back",
// just comment out these statements:
//		newSpeed=-newSpeed;
//		setNewSpeed();
//
// and uncomment these statements:
//		salvos[i].raindrops[j].x=-1;
//		salvos[i].raindrops[j].z=-1;
//
// I'm going to work on smoothening out the transitions
// so that I hope it'll look better.
void updateSalvos()
{
    for(int i=0;i<cube.size;i++)
    {
        int offCube=true;
      
        for(int j=0;j<MAX_POINTS;j++)
        {
            salvos[i].raindrops[j].y+=newSpeed;	//SPEED;
          	if(newSpeed>0)
            {
                if(salvos[i].raindrops[j].y<cube.size)
                  	offCube=false;
                else
                {
                  	newSpeed=-newSpeed;
                    //salvos[i].raindrops[j].x=-1;
                    //salvos[i].raindrops[j].z=-1;
                }
            }
          	else
                if(salvos[i].raindrops[j].y>0)
                    offCube=false;
                else
                {
                  	newSpeed=-newSpeed;
                    //salvos[i].raindrops[j].x=-1;
                    //salvos[i].raindrops[j].z=-1;
                }
        }

      	if(offCube)
        {
            salvos[i].dead=true;
          	setNewSpeed();
        }
      	else
            for(int j=0;j<MAX_POINTS;j++)
            {
                salvos[i].raindrops[j].x=-1;
                salvos[i].raindrops[j].z=-1;
            }
          
    }
}
 
void initMicrophone()
{
  pinMode(GAIN_CONTROL, OUTPUT);
  digitalWrite(GAIN_CONTROL, LOW);
}

void setNewSpeed()
{
    int rndSpeed=0+(rand()%7);
    switch(rndSpeed)
    {
      case 0:
        newSpeed=0.5;
        break;
      case 1:
        newSpeed=-0.5;
        break;
      case 2:
        newSpeed=0.15;
        break;
      case 3:
        newSpeed=-0.15;
        break;
      case 4:
        newSpeed=0.25;
        break;
      case 5:
        newSpeed=-0.25;
        break;
      case 6:
        newSpeed=0.35;
        break;
      case 7:
        newSpeed=-0.35;
        break;
      default:
        newSpeed=SPEED;
        break;
    }
}

void initSalvos()
{
    for(int i=0;i<cube.size;i++)
    {
        for(int j=0;j<MAX_POINTS;j++)
        {
            salvos[i].raindrops[j].x=-1;
            salvos[i].raindrops[j].z=-1;
        }
        salvos[i].dead=true;
    }
}