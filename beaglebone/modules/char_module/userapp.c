#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/bbt_charmod"

int main(void)
{
	int fd;		// File descriptor
	int i;
	char ch;
	char write_buff[100];
	char read_buff[100];

	// Open the file
	fd = open(DEVICE, O_RDWR);		// Open for reading and writing
	if (fd == -1)
	{
		printf("File %s either does not exist or has been locked by another process\n", DEVICE);
		exit(-1);
	}

	// Ask for user input
	printf("r = read from device\nw = write to device\nenter command: ");
	scanf("%c", &ch);

	switch (ch)
	{
		case 'w':
			printf("Please enter your data: ");
			scanf(" %[^\n]", write_buff);
			write(fd, write_buff, sizeof(write_buff));
			break;

		case 'r':
			read(fd, read_buff, sizeof(read_buff));
			printf("Device data: %s\n", read_buff);
			break;

		default:
			printf("Unknown command\n");
			break;
	}
	close(fd);
	printf("File closed and closing app\n");

	return 0;
}