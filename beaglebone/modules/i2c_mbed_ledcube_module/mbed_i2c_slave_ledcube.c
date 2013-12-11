#include "mbed.h"

DigitalOut din(p15);
DigitalOut clk(p16);
DigitalOut le(p17);

enum CUBE_CONSTANTS {
    CUBE_LEVELS = 5,
    DATA_SIZE = 32,
    CUBE_LEVEL_DELAY = 4     // ms (4 seems to be minimum)
};

void outputData(int data)
{
    int i;

    for (i = 0; i < DATA_SIZE; i++) {
        clk = 0;
        din = ((data >> i) & 0x0000001);
        clk = 1;
    }
    clk = 0;
    le = 1;
    wait_us(1);
    le = 0;
}

int main()
{
    int cubedata[CUBE_LEVELS] = {   // 32 bits per level and 5 levels
        0xF0000001,                 // Some hardcoded test data
        0xE8000002,
        0xD8000004,
        0xB8000008,
        0x78000010
    };
    unsigned char ilevel = 0;

    clk = 0;
    din = 0;
    le = 0;

    while(1)
    {
        outputData(cubedata[ilevel]);
        
        wait_ms(CUBE_LEVEL_DELAY);        
        ilevel = (ilevel + 1) % 5;
    }
}
