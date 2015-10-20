
#include "beta-cube-library.h"

char* vizName="    Sound Balloon";
int vizId=192;
#include <math.h>

float maxAmplitude=0;

#define NUM_SAMPLES 100
int samples[NUM_SAMPLES];
int sampleIndex=0;
Cube cube=Cube();
void setup() {

  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {
    cube.background(black);
  long runningAverage=0;
  for(int i=0;i<NUM_SAMPLES;i++)
  {
	int mic=analogRead(MICROPHONE);
   	int amplitude=abs(mic-2048);  
    runningAverage+=amplitude;
  //the microphone values from the ADC range from
  //0 to 4095.  The mic is an AC signal, biased around 1.65v,
  //so a flat line from the mic reads as 2048.  If we want the amplitude
  //of the audio signal, we have to look at the difference between the signal
  //and a flat DC signal, so I look at how far the sample is from 2048  
  if(amplitude>maxAmplitude)
    	maxAmplitude=amplitude;
}
  	runningAverage/=NUM_SAMPLES;
  	maxAmplitude*=.98;  
   //this is an autoscaling function that continuously zooms
   //in on a low-amplitude signal, and then immediately rescales if
  //the signal jumps in amplitude

	cube.shell(cube.center, map(runningAverage, 0, maxAmplitude, 0, cube.size*3/4), Color(map(runningAverage,0,maxAmplitude,30,150),0,0));
  
    cube.show();
}



