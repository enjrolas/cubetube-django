
#include "beta-cube-library.h"

char* vizName="FFT Music Input";
int vizId=30;
#include "neopixel/neopixel.h"
#include <math.h>
//set up the pin that controls the LEDs, the type of LEDs (WS2812B) and the number of LEDs in the cube (8*8*8=512)
#define PIXEL_PIN D0
#define PIXEL_COUNT 512
#define PIXEL_TYPE WS2812B
#define SIDE 8

SYSTEM_MODE(SEMI_AUTOMATIC);  //don't connect to the internet on boot
#define BUTTON D2 //press this button to connect to the internet
#define MODE D3
#define MICROPHONE 12
#define GAIN_CONTROL 11

bool onlinePressed=false;
bool lastOnline=true;

int maxBrightness=55;
int point;
float max=0;

#define M 4
/*  datatype definitions
*/

typedef struct{
    unsigned char red, green, blue;
} color;


color black;
/******************************
 * function definitions
 * ***************************/
void background(color col);
color getPixel(int x, int y, int z);
void setPixel(int x, int y, int z, color col);
color colorMap(float val, float min, float max);
color lerpColor(color a, color b, int val, int min, int max);


Adafruit_NeoPixel strip=Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
float samples[(int)pow(2,M)];
float real[(int)pow(2,M)];
float imaginary[(int)pow(2,M)];


void setup() {
    pinMode(7,OUTPUT);
    digitalWrite(7, HIGH);
 Serial.begin(115200);
 initCube();
 initCloudButton();
 initMicrophone();
 point=0;
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

//sets up the online/offline switch
void initCloudButton()
{
  //set the input mode for the 'connect to cloud' button
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);
    if(!digitalRead(MODE))
        WiFi.listen();
    //a.k.a. onlinePressed is HIGH when the switch is set to 'online' and LOW when the switch is set to 'offline'
    onlinePressed=digitalRead(BUTTON);
    if(onlinePressed)
        Spark.connect();
}

//checks to see if the 'online/offline' switch is switched
void checkCloudButton()
{
    //if the 'connect to cloud' button is pressed, try to connect to wifi.  
    //otherwise, run the program
    //note -- how does this behave when there are no wifi credentials loaded on the spark?

    //onlinePressed is HIGH when the switch is _not_ connected and LOW when the switch is connected
    //a.k.a. onlinePressed is HIGH when the switch is set to 'online' and LOW when the switch is set to 'offline'
    onlinePressed=digitalRead(BUTTON);
    
    if((!onlinePressed)&&(lastOnline))  //marked as 'online'
    {
        lastOnline=onlinePressed;
        Spark.connect();
    }    

    else if((onlinePressed)&&(!lastOnline))  //marked as 'offline'
    {
        lastOnline=onlinePressed;
        Spark.disconnect();
    }

    lastOnline=onlinePressed;
    
    if(!digitalRead(MODE))
        WiFi.listen();
}

void loop() {
    //if the 'connect to cloud' button is pressed, try to connect to wifi.  
    //otherwise, run the program
    checkCloudButton();

    for(int i=0;i<pow(2,M);i++)
    {
        real[i]=analogRead(MICROPHONE)-2048;
        Serial.print(real[i]);
//        Serial.print("   ");
        imaginary[i]=0;
    }
    Serial.println();

        /*
    for(int i=0;i<pow(2,M);i++)
    {
        float currentSample;
        if((point-i)<0)
            currentSample=samples[(point-i)+(int)pow(2,M)];
        else
            currentSample=samples[(point-i)];
        real[i]=currentSample;
        imaginary[i]=0;
    //    Serial.print(real[i]);
    //    Serial.print(" ");
    }
    */
//    Serial.println();
    FFT(1, M, real, imaginary);
    for(int i=0;i<pow(2,M);i++)
    {
        imaginary[i]=sqrt(pow(imaginary[i],2)+pow(real[i],2));
//        Serial.print(imaginary[i]);
        if(imaginary[i]>max)
            max=imaginary[i];
    }
    if(max>100)
        max--;
//    Serial.println();
    for(int i=0;i<pow(2,M)/2;i++)
    {
        imaginary[i]=SIDE*imaginary[i]/max;
        int y;
        for(y=0;y<=imaginary[i];y++)
            setPixel(i,y,SIDE-1,colorMap(y,0,SIDE));
        for(;y<SIDE;y++)
            setPixel(i,y,SIDE-1,black);
    }
    for(int z=0;z<SIDE-1;z++)
        for(int x=0;x<SIDE;x++)
            for(int y=0;y<SIDE;y++)
                setPixel(x,y,z,getPixel(x,y,z+1));

    if(max<10)
        max-=5;
    point++;
    if(point>=pow(2,M))
        point-=pow(2,M);

    strip.show();
}


//sets a pixel at position (x,y,z) to the col parameter's color
void setPixel(int x, int y, int z, color col)
{
    int index = (z*SIDE*SIDE) + (x*SIDE) + y;
        strip.setPixelColor(index,strip.Color(col.red,  col.green, col.blue));
    
}


//returns the color value currently displayed at the x,y,z location
color getPixel(int x, int y, int z)
{
    int index = (z*SIDE*SIDE) + (x*SIDE) + y;
    uint32_t col=strip.getPixelColor(index);
    color pixelColor;
    pixelColor.red=(col>>16)&255;
    pixelColor.green=(col>>8)&255;
    pixelColor.blue=col&255;
    return pixelColor;
}

void initCube()
{
    black.red=0;
    black.green=0;
    black.blue=0;
}


void background(color col)
{
    for(int x=0;x<SIDE;x++)
        for(int y=0;y<SIDE;y++)
            for(int z=0;z<SIDE;z++)
                setPixel(x,y,z, col);    
}


//returns a color from a set of colors fading from blue to green to red and back again
//the color is returned based on where the parameter *val* falls between the parameters
//*min* and *max*.  If *val* is min, the function returns a blue color.  If *val* is halfway
//between *min* and *max*, the function returns a yellow color.  
color colorMap(float val, float min, float max)
{
  float range=1024;
  val=range*(val-min)/(max-min);
  color colors[6];
  colors[0].red=0;
  colors[0].green=0;
  colors[0].blue=maxBrightness/4;
  
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
  
  if (val<=range/6)
    return(lerpColor(colors[0], colors[1], val, 0, range/6));
  else if (val<=2*range/6)
    return(lerpColor(colors[1], colors[2], val, range/6, 2*range/6));
  else if (val<=3*range/6)
    return(lerpColor(colors[2], colors[3], val, 2*range/6, 3*range/6));
  else if (val<=4*range/6)
    return(lerpColor(colors[3], colors[4], val, 3*range/6, 4*range/6));
  else if (val<=5*range/6)
    return(lerpColor(colors[4], colors[5], val, 4*range/6, 5*range/6));
  else
    return(lerpColor(colors[5], colors[0], val, 5*range/6, range));
}


//returns a color that's an interpolation between colors a and b.  The color
//is controlled by the position of val relative to min and max -- if val is equal to min,
//the resulting color is identical to color a.  If it's equal to max, the resulting color 
//is identical to color b.  If val is (max-min)/2, the resulting color is the average of
//color a and color b
color lerpColor(color a, color b, int val, int min, int max)
{
    color lerped;
    lerped.red=a.red+(b.red-a.red)*(val-min)/(max-min);
    lerped.green=a.green+(b.green-a.green)*(val-min)/(max-min);
    lerped.blue=a.blue+(b.blue-a.blue)*(val-min)/(max-min);
    return lerped;
}


void initMicrophone()
{
  pinMode(GAIN_CONTROL, OUTPUT);
  digitalWrite(GAIN_CONTROL, LOW);
}


short FFT(short int dir,int m,float *x,float *y)
{
   int n,i,i1,j,k,i2,l,l1,l2;
   float c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   n = 1;
   for (i=0;i<m;i++) 
      n *= 2;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1; 
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<n;i++) {
         x[i] /= n;
         y[i] /= n;
      }
   }

   return(0);
}