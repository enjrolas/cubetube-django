
#include "beta-cube-library.h"

char* vizName="         Remote_Control_Loop";
int vizId=555;
#include <vector>
#include <stdint.h>

Cube cube = Cube();
String flag;

Color g = { 0, 255, 0 };
Color lg= { 0, 25,  0 };

std::vector<Point> Circle = {
  { 0, 4, 3 },
  { 1, 6, 3 },
  { 3, 7, 3 },
  { 5, 6, 3 },
  { 6, 4 ,3 },
  { 5, 2 ,3 },
  { 3, 1, 3 },
  { 1, 2, 3 }
};

void setup()
{
  cube.begin();
  cube.background(black);
  
  flag = "none";
  Spark.function( "led", ledToggle );
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
  //Breath_All_Color();
  
  if( flag == "reset" )
  {
    cube.background( black );
    Breath_All_Color();
  }
  
  if( flag == "none" )
  {
    cube.background( black );
    cube.setVoxel(4,4,4, Color( 0,0,0) );
    cube.show();
  }
  //-----------------------------------
  if( flag == "on" )
  {
    //cube.background( black );
    //cube.setVoxel(4,4,4,Color(255,255,0));
    //cube.show();
    //Line();
    DrawCircle();
  }
  //------------------------------------
  if( flag == "off" )
  {
    Red_Alert();
  }
}

int ledToggle( String _command )
{
 if( _command == "on" || _command == "off" || _command == "reset" )
 {
	flag = _command;
 }
  else
  {
    flag = "none";
    return -1;
  }
}

int Breath_All_Color()
{
  for( int B = 0; B < 256; B++ )
  {
    for( int G = 0; G < 256; G++ )
    {
      for( int R = 0; R < 256; R++ )
      {
        cube.background( Color( R, G, B ) );
        //cube.setVoxel( 3, 3, 3, Color( R, G, B ) );
        cube.show();
        delay( 10 );
      } 
    }
  }
  
}
  
  
int Red_Alert()
{
  cube.background( black ); 
  int smoothness = 8;
  for( int i = 1; i <= 255; i++ )
  {
    cube.background( Color( i/smoothness, 0, 0 ) );
    cube.show();
    delay( 10 );
  }
  //cube.background( Color( 255, 0, 0 ) );
  //cube.show();
  //delay( 10 );
  
  for( int i = 254; i > 0; i-- )
  {
    cube.background( Color( i/smoothness, 0, 0 ) );
    cube.show();
    delay( 5 );
  } 
}

int Line()
{
  cube.background( black );
  for( int i = 6; i >0; i-- )
  {
    cube.setVoxel(  i, 0, 4, Color( 0, 255/i/i, 0 ) );
  }
  cube.show();
  delay(3000);
}

int DrawCircle()
{
  cube.setVoxel( 3, 4, 3, white );
  cube.show();
  
  cube.setVoxel( Circle[0], g     );
  cube.setVoxel( Circle[7], lg );
  cube.show();
  delay(50);
  cube.setVoxel( Circle[0], black );
  cube.setVoxel( Circle[7], black );
  
  for( int i = 1; i < 8; i++ )
  {
    cube.setVoxel( Circle[i],   g     );
    cube.setVoxel( Circle[i-1], lg );
    cube.show();
    delay(50);
    
    cube.setVoxel( Circle[i],   black );
    cube.setVoxel( Circle[i-1], black );
  }
}
  
  
//  cube.setVoxel( 0, 4, 3, green );
//  cube.show();
//  delay(10);
//  cube.setVoxel( 0, 4, 3, black );
  
//  cube.setVoxel( 1, 6, 3, green );
//  cube.show();
//  delay(10);
//  cube.setVoxel( 1, 6, 3, black );
  
//  cube.setVoxel( 3, 7, 3, green );
//  cube.show();
//  delay(10);
//  cube.setVoxel( 3, 7, 3, black );
  
//  cube.setVoxel( 5, 6, 3, green );
//  cube.show();
//  delay(10);
//  cube.setVoxel( 5, 6, 3, black );
  
//  cube.setVoxel( 6, 4, 3, green );
//  cube.show();
// delay(10);
//  cube.setVoxel( 6, 4, 3, black );
  
//  cube.setVoxel( 5, 2, 3, green );
//  cube.show();
//  delay(10);
//  cube.setVoxel( 5, 2, 3, black );
  
//  cube.setVoxel( 3, 1, 3, green );
//  cube.show();
//  delay(10);
//  cube.setVoxel( 3, 1, 3, black );
  
//  cube.setVoxel( 1, 2, 3, green );
//  cube.show();
//  delay(10);
//  cube.setVoxel( 1, 2, 3, black );

//}