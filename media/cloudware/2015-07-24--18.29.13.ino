
#include "beta-cube-library.h"

#include <math.h>
 
#define SIDE 8
#define PIXEL_PIN D0
#define PIXEL_COUNT 512
#define PIXEL_TYPE WS2812B
Adafruit_NeoPixel strip=Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
#define X 13
#define Y 14
#define Z 15
 
#define MICROPHONE 12
#define GAIN_CONTROL 11
#define MAX_POINTS 20
#define SPEED 0.22
#define MIN_SALVO_SPACING 100
 
int threshhold;
int maximum;
int minimum;
float sensitivity=0.5;
int maxAmplitude=0;
bool aboveThreshhold=false;
int timeAboveThreshhold;

int maxBrightness=50;

/************************
 * type definitions     *
 * *********************/
 
typedef struct{
    Point raindrops[MAX_POINTS];
    bool dead;
} salvo;
 
Color rainColor;
salvo salvos[SIDE];
 
/************************
 * function definitions *
 * *********************/
 
void background(Color col);
Color getPixel(int x, int y, int z);
void setPixel(int x, int y, int z, Color col);
Color colorMap(float value, float minimum, float maximum);
Color lerpColor(Color a, Color b, int value, int minimum, int maximum);
 
 
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //Serial.begin(115200);
  //Serial.println("initializing...");
  initMicrophone();
  initSalvos();
  rainColor.red=100;
  rainColor.green=0;//80;
  rainColor.blue=100;
}
 
void loop() {
    background(black);
    checkMicrophone();
    updateSalvos();
    drawSalvos();
    strip.show();
    Spark.process();
  	delay(20);
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
    for(i=0;((i<SIDE)&&(!salvos[i].dead));i++)
        ;
    if(i<SIDE)
    {
        if(amplitude>maxAmplitude)
            maxAmplitude=amplitude;
        int numDrops=map(amplitude,0, maxAmplitude,0, MAX_POINTS);
        for(int j=0;j<numDrops;j++)
        {
            salvos[i].raindrops[j].x=1+(rand()%6);
            salvos[i].raindrops[j].y=((rand()%10)-5)/10;
            salvos[i].raindrops[j].z=1+(rand()%6);
            salvos[i].dead=false;
        }
        for(int j=numDrops;j<MAX_POINTS;j++)
        {
            salvos[i].raindrops[j].x=-1;
            salvos[i].raindrops[j].z=-1;
        }
 
    }
   
}

void drawSalvos()
{
    for(int i=0;i<SIDE;i++)
        if(!salvos[i].dead)
            for(int j=0;j<MAX_POINTS;j++)
                setPixel(salvos[i].raindrops[j].x, salvos[i].raindrops[j].y, salvos[i].raindrops[j].z, rainColor);
}
 
void updateSalvos()
{
    for(int i=0;i<SIDE;i++)
    {
        int offCube=true;
        for(int j=0;j<MAX_POINTS;j++)
        {
            salvos[i].raindrops[j].y+=SPEED;
            if(salvos[i].raindrops[j].y<SIDE)
                offCube=false;
            else
            {
                salvos[i].raindrops[j].x=-1;
                salvos[i].raindrops[j].z=-1;
            }
        }
        if(offCube)
            salvos[i].dead=true;
    }
}
 
void initMicrophone()
{
  pinMode(GAIN_CONTROL, OUTPUT);
  digitalWrite(GAIN_CONTROL, LOW);
}
 
void initSalvos()
{
    for(int i=0;i<SIDE;i++)
    {
        for(int j=0;j<MAX_POINTS;j++)
        {
            salvos[i].raindrops[j].x=-1;
            salvos[i].raindrops[j].z=-1;
        }
        salvos[i].dead=true;
    }
}
 
//sets a pixel at position (x,y,z) to the col parameter's color
void setPixel(int x, int y, int z, Color col)
{
    int index = (z*SIDE*SIDE) + (x*SIDE) + y;
    if(index<512)
        strip.setPixelColor(index,strip.Color(col.red,  col.green, col.blue));
   
}

//returns the color value currently displayed at the x,y,z location
Color getPixel(int x, int y, int z)
{
    int index = (z*SIDE*SIDE) + (x*SIDE) + y;
    uint32_t col=strip.getPixelColor(index);
    Color pixelColor;
    pixelColor.red=(col>>16)&255;
    pixelColor.green=(col>>8)&255;
    pixelColor.blue=col&255;
    return pixelColor;
}
 
void background(Color col)
{
    for(int x=0;x<SIDE;x++)
        for(int y=0;y<SIDE;y++)
            for(int z=0;z<SIDE;z++)
                setPixel(x,y,z, col);    
}
 
//returns a color from a set of colors fading from blue to green to red and back again
//the color is returned based on where the parameter *value* falls between the parameters
//*minimum* and *maximum*.  If *value* is minimum, the function returns a blue color.  If *value* is halfway
//between *minimum* and *maximum*, the function returns a yellow color.  
Color colorMap(float value, float minimum, float maximum)
{
  float range=1024;
  value=range*(value-minimum)/(maximum-minimum);
  Color colors[6];
  colors[0].red=0;
  colors[0].green=0;
  colors[0].blue=maxBrightness;
 
  colors[1].red=0;
  colors[1].green=maxBrightness;
  colors[1].blue=maxBrightness;
 
  colors[2].red=0;
  colors[2].green=maxBrightness;
  colors[2].blue=0;
 
  colors[3].red=maxBrightness;
  colors[3].green=maxBrightness;
  colors[3].blue=0;
 
  colors[4].red=maxBrightness;
  colors[4].green=0;
  colors[4].blue=0;
 
  colors[5].red=maxBrightness;
  colors[5].green=0;
  colors[5].blue=maxBrightness;
 
  if (value<=range/6)
    return(lerpColor(colors[0], colors[1], value, 0, range/6));
  else if (value<=2*range/6)
    return(lerpColor(colors[1], colors[2], value, range/6, 2*range/6));
  else if (value<=3*range/6)
    return(lerpColor(colors[2], colors[3], value, 2*range/6, 3*range/6));
  else if (value<=4*range/6)
    return(lerpColor(colors[3], colors[4], value, 3*range/6, 4*range/6));
  else if (value<=5*range/6)
    return(lerpColor(colors[4], colors[5], value, 4*range/6, 5*range/6));
  else
    return(lerpColor(colors[5], colors[0], value, 5*range/6, range));
}

//returns a color that's an interpolation between colors a and b.  The color
//is controlled by the position of value relative to minimum and maximum -- if value is equal to minimum,
//the resulting color is identical to color a.  If it's equal to maximum, the resulting color
//is identical to color b.  If value is (maximum-minimum)/2, the resulting color is the average of
//color a and color b
Color lerpColor(Color a, Color b, int value, int minimum, int maximum)
{
    Color lerped;
    lerped.red=a.red+(b.red-a.red)*(value-minimum)/(maximum-minimum);
    lerped.green=a.green+(b.green-a.green)*(value-minimum)/(maximum-minimum);
    lerped.blue=a.blue+(b.blue-a.blue)*(value-minimum)/(maximum-minimum);
    return lerped;
}