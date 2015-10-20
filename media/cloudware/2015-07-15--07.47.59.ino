
#include "beta-cube-library.h"

char* vizName="    test1";
int vizId=583;
float pos=0;
float posInc=0.1;
int mode=0;
String message=" ";

Cube cube=Cube();

void setup(){
  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop(){
  cube.background(black);
  switch(mode) {

    case(0):    
    cube.text.showChar('L', new PVector(3.5, 0, 3.5), new PVector(0, pos), cube.colorMap(frameCount%1000, 0, 1000));
    break;  
    case(1):
    cube.text.showChar('3', new PVector(1, 3.5, 3.5), new PVector(0, 3.5), new PVector(pos, 0), cube.colorMap(frameCount%1000, 0, 1000));
    break;  
    case(2):
    cube.text.showChar('D', new PVector(1.5, 0, 3.5), new PVector(0, 0), new PVector(0, -pos), cube.colorMap(frameCount%1000, 0, 1000));
    break;  
  }

  pos+=posInc;
  if (pos>(message.length()+1)*8)
  {
    pos=0;
    mode++;
    if(mode>2)
      mode=0;
  }
  cube.show();
}
