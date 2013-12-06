#include <mbed.h>

I2CSlave slave(p9, p10);    //An I2C Slave, used for communicating with an I2C Master device
DigitalOut led_read(LED1);
DigitalOut led_write(LED2);
 
enum command {
    SET_PAGE_ADDRESS = 0x00,
    WRITE_PAGE = 0x01
};

int main() 
{
    char data[10][32];      // Memory pages
    int pageAddress = 0;
    char buffer[34];

    printf("Serving as slave on address 0x68\r\n");
    led_read = 0;
    led_write = 0;

    // Configure slave
    slave.frequency(100000);
    slave.address(0xD0);        // 0x68 on linux
    
    // Eternity
    while (1)
    {
        int i = slave.receive();

        switch (i)
        {
            case I2CSlave::WriteAddressed:      // The master is writing to this slave           
                
                if (slave.read(buffer, 34))
                {
                    led_write = !led_write;
                    
                    switch (buffer[0])      // Command byte
                    {
                        case SET_PAGE_ADDRESS:  // This is for a read action

                            if (buffer[1] < 10) {
                                pageAddress = buffer[1];
                                printf("Setting page address to %x\r\n", pageAddress);
                            }
                            else {
                                printf("Invalid page address\r\n");
                            }
                            break;

                        case WRITE_PAGE:

                            if (buffer[1] < 10) {
                                pageAddress = buffer[1];
                                memcpy(data[pageAddress], (buffer+2), 32);
                                printf("Got request to write a page on address %d with the data: %s\r\n", pageAddress, (buffer+2));
                            }
                            else {
                                printf("Invalid page address\r\n");
                            }
                            break;

                        default:
                            printf("Unknown command byte\r\n");
                            break;
                    }
                }
                else
                {
                    printf("Could not retrieve i2c data\r\n");
                }
                break;

            case I2CSlave::ReadAddressed:       // the master has requested a read from this slave

                if (slave.write(data[pageAddress], 32))
                {
                    led_read = !led_read;
                    printf("Got request to read a page on address %d with the data: %s\r\n", pageAddress, data[pageAddress]);
                }
                else
                {
                    printf("Could not send i2c data\r\n");
                }
                break;
        }
        
        for (int i = 0; i < 34; i++) {
            buffer[i] = 0;
        }
    }
}
