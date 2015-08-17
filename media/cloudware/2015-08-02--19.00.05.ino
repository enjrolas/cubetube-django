
#include "beta-cube-library.h"

// Blinkenlights
// Dennis Williamson - 2015-06-23

// https://en.wikipedia.org/wiki/Blinkenlights

// this demo is not intended to represent any real hardware

// registers
#define ADDRESS 7
#define OP 6
#define DATA 5
#define REG0 4
#define REG1 3
#define REG2 2
#define REG3 1
#define STATUS 0

// status indicators
#define POWER 0
#define CLOCK 1
#define JMP 2
#define OVERFLOW 3
#define ERROR 4
#define STEP 6
#define HALT 7

#define ON true
#define OFF false

#define BYTE 256

#define RUN_RATE 150
#define STEP_RATE 1000 + random(-800, 800)
#define HALT_DELAY 3000

// randomness
#define OFTEN 16
#define SELDOM 128

int rate = RUN_RATE;
int steps = 0;

// circular buffers
#define BUFFER_SIZE 8
int addr_buffer[BUFFER_SIZE];
bool jmp_buffer[BUFFER_SIZE];
int op_buffer[BUFFER_SIZE];
int data_buffer[BUFFER_SIZE];
int register0[BUFFER_SIZE];
int register1[BUFFER_SIZE];
int register2[BUFFER_SIZE];
int register3[BUFFER_SIZE];

int buffer_pos = 0;

int addr = 0;

bool clock_state = true;

int data = 0;
int prev_data = 0;
int data_ctl = 0;

Cube cube = Cube();
int size = 0;

void dec2bin(int number, int y, int z, Color color);
void indicate(int indicator, bool on);
void next_addr();
void op_data();
void clock_tick();
void halt_step_run();
void display_buffers();

void setup()
{
	cube.begin();
    cube.background(black);
	size = cube.size;
  	indicate(POWER, ON);
  	//cube.maxBrightness = 50;
}

void loop()
{
	display_buffers();
  	next_addr();
  	op_buffer[buffer_pos] = random(BYTE);

  	op_data();

  	// do registers here

	prev_data = data;

    buffer_pos++;
    //if (buffer_pos >= BUFFER_SIZE) { buffer_pos = 0; }
  	buffer_pos %= BUFFER_SIZE;

  	halt_step_run();

  	clock_tick();

  	cube.show();

    delay(rate);
}

void clock_tick()
{
  	indicate(CLOCK, clock_state);
  	clock_state = !clock_state;
}

void halt_step_run()
{
  	if (!random(SELDOM) && !steps)
    {
	  	// halt (but don't catch fire)
      	indicate(HALT, ON);
      	delay(HALT_DELAY);
      	indicate(HALT, OFF);
    }
  	else if (!random(SELDOM))
    {
      	// enter step mode
      	steps = random(32);
    }

	if (steps > 0)
    {
      	steps--;
      	rate = STEP_RATE;
      	indicate(STEP, ON);
    }
  	else
    {
      	rate = RUN_RATE;
	    indicate(STEP, OFF);
    }
}

void next_addr()
{
    if (random(OFTEN))
    {
      	addr++;
      	//if (addr > 255) { addr = 0; }
      	addr %= BYTE;
        jmp_buffer[buffer_pos] = OFF;
    }
    else
    {
      	// occasionally execute JMP instruction
      	addr = random(BYTE);
        // light JMP indicator
        jmp_buffer[buffer_pos] = ON;
    }

    addr_buffer[buffer_pos] = addr;
}

void op_data()
{
  	data_ctl = random(OFTEN);
  	if (data_ctl < 3)
    {
      	data = 0;
    }
  	else if (data_ctl < 5)
    {
      	data = prev_data;
    }
  	else
    {
  		data = random(BYTE);
    }
  	data_buffer[buffer_pos] = data;
}

void display_buffers()
{
  	int loc;
  	for(int i = BUFFER_SIZE - 1, brightness = 10; i >= 0; i--, brightness = 1)
    {
      	loc = (buffer_pos + i) % BUFFER_SIZE;

      	dec2bin(addr_buffer[loc], ADDRESS, i, Color(2 * brightness, 0, 0));
		cube.setVoxel(JMP, STATUS, i, Color(0, jmp_buffer[loc] * brightness, 0));

	    dec2bin(op_buffer[loc], OP, i, Color(0, 1 * brightness, 0));

      	dec2bin(data_buffer[loc], DATA, i, Color(3 * brightness, 2 * brightness, 0));

	    dec2bin(register0[loc], REG0, i, Color(2 * brightness, 0, 0));
	    dec2bin(register1[loc], REG1, i, Color(0, 1 * brightness, 0));
	    dec2bin(register2[loc], REG2, i, Color(2 * brightness, 0, 0));
	    dec2bin(register3[loc], REG3, i, Color(3 * brightness, 2 * brightness, 0));
    }
}

void indicate(int indicator, bool on)
{
    Color color;

  	if (on)
    {
        if (indicator % 2) // even indicators are green, odd are red
        {
            color = Color(10, 0, 0);
        }
        else
        {
            color = Color(0, 10, 0);
        }
        // light indicator
    	cube.setVoxel(indicator, STATUS, 7, color);
    }
  	else
    {
      	// extinguish indicator
		cube.setVoxel(indicator, STATUS, 7, black);
    }
  	cube.show();
}

void dec2bin(int number, int y, int z, Color color)
{
	byte mask = 128;
	for (int bit = 7; bit >= 0; bit--)
    {
    	if (number & mask)
        {
        	cube.setVoxel(7 - bit, y, z, color);
        }
      	else
        {
        	cube.setVoxel(7 - bit, y, z, black);
        }
    	mask >>= 1;
    }
}