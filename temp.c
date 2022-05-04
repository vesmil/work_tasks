#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


int main()
{
	int port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);

	if (port < 0) { return 1; }

	// flush previous bytes
	int result = tcflush(port, TCIOFLUSH);
	if (result)	{ perror("tcflush failed");}
	
	// Get current configuration
	struct termios options;
	result = tcgetattr(port, &options);
	if (result) { close(port); return 2; }
        
    // Set flags and timeouts
	options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
	options.c_oflag &= ~(ONLCR | OCRNL);
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

	options.c_cc[VTIME] = 1;
	options.c_cc[VMIN] = 0;
	
	// Set baud rate - TODO variable
	cfsetospeed(&options, B9600);
	cfsetispeed(&options, B9600);

	result = tcsetattr(port, TCSANOW, &options);
	if (result) { close(port); return 3; }
  	
  	unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
	write(port, msg, sizeof(msg));
	
	size_t buffer[6];
	
	int bytes_read = read(port, &buffer, sizeof(buffer));
	printf("Read %i bytes. Received message: %s\n", bytes_read, buffer);
	
	close(port);

}
