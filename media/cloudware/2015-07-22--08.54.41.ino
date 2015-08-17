
#include "beta-cube-library.h"

UDP Udp;
unsigned count = 0;
Cube cube=Cube();
char data[512];

char localIP[24];
char macAddress[20];
int port;
int lastUpdated=0;

void initSparkVariables();
void updateNetworkInfo();

void setup() 
{
  port=2222;
  cube.begin();
  cube.show(); // Initialize all pixels to 'off'
  Udp.begin (port);
  Serial.begin(115200);
  Serial.println("initializing...");
  delay(5000);
  updateNetworkInfo();
  initSparkVariables();
}

//initializes the shared variables and functions that are accessible through the spark API
//this makes the core's local IP address, MAC address and port accessible to streaming programs
//that have the access token for the core owner's account
//The function setPort lets a streaming program set the port on which the core will listen for streaming packets
void initSparkVariables()
{
    Spark.variable("IPAddress", localIP, STRING);
    Spark.variable("MACAddress", macAddress, STRING);
    Spark.variable("port", &port, INT);
}

//updates the local IP address and mac address and stores them in global strings
//if those strings are Spark variables (using the Spark.variable() function), they'll be accessible to the wider world
//This is all kinds of helpful anytime you're working on a project that uses network communicqtion (streaming, etc)

void updateNetworkInfo()
{
    IPAddress myIp = WiFi.localIP();
    sprintf(localIP, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
    byte macAddr[6];
    WiFi.macAddress(macAddr);
    sprintf(macAddress, "%02x:%02x:%02x:%02x:%02x:%02x",macAddr[5],macAddr[4],macAddr[3],macAddr[2],macAddr[1],macAddr[0]);

    //print it out the serial port
    Serial.print("local IP Address: ");
    Serial.println(localIP);
    Serial.print("MAC address:  ");
    Serial.println(macAddress);
}


void loop (void)
{
int32_t bytesrecv = Udp.parsePacket();

if(millis()-lastUpdated>5000)  //update the network settings every minute
{
    updateNetworkInfo();
    lastUpdated=millis();
}

if (bytesrecv==512) {
    Udp.read(data,bytesrecv);
    for(int x=0;x<cube.size;x++)
        for(int y=0;y<cube.size;y++)
            for(int z=0;z<cube.size;z++)
            {
                int index = z*64 + y*8 + x;
              	Color voxelColor=Color((data[index]&0xE0)>>2, (data[index]&0x1C)<<1, (data[index]&0x03)<<4);
              	cube.setVoxel(x,y,z,voxelColor);
            }
        
    }
    cube.show();
}