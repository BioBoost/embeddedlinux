#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>		// For sleep

#define DEVICE "/dev/mbed_cube"

#define BUFFER_SIZE 20
#define CUBE_LEVELS 5
#define CUBE_LEVEL_LEDS 25

int main(void)
{
	int fd;		// File descriptor
	unsigned int l, i, c;
	char ch;
	unsigned int cubedata[CUBE_LEVELS];

	if (sizeof(int) < 4) {
		printf("Cant create cube data on system with less than 4 bytes per int\n");
		return -1;
	}

	// Open the file
	fd = open(DEVICE, O_WRONLY);		// Open for writing only
	if (fd == -1)
	{
		printf("File %s either does not exist or has been locked by another process\n", DEVICE);
		exit(-1);
	}

	for (l = 0; l < CUBE_LEVELS; l++) {
		cubedata[l] = 0xF8000000;	// All clear
	}
	write(fd, ((char*)cubedata), BUFFER_SIZE);
	sleep(2);

	// Light all leds one by one
	for (l = 0; l < CUBE_LEVELS; l++)
	{
		for (c = 0; c < CUBE_LEVEL_LEDS; c++)
		{
			cubedata[l] = 0xF8000000 ^ (0x08000000 << l);
			cubedata[l] += ((0x00000001 << c) - 1);
				
			write(fd, ((char*)cubedata), BUFFER_SIZE);
			sleep(1);

			for (i = c; i < CUBE_LEVEL_LEDS; i++) 
			{
				cubedata[l] = 0xF8000000 ^ (0x80000000 >> l);
				cubedata[l] += ((0x00000001 << c) - 1) + (0x00000001 << i);
				
				write(fd, ((char*)cubedata), BUFFER_SIZE);
				sleep(1);
			}
		}
	}

	for (l = 0; l < CUBE_LEVELS; l++) {
		cubedata[l] = 0xF8000000;	// All clear
	}
	write(fd, ((char*)cubedata), BUFFER_SIZE);
	sleep(2);

	close(fd);
	printf("File closed and closing app\n");

	return 0;
}