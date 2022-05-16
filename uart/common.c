#include "common.h"

const speed_t baud_rates[] = {B230400, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200};

int port_init(int advanced_init, char* device) {	

	if (advanced_init) {
		printf("Choose a tty file (default %s): ", device);
		
		char input_device[64];
		if (fgets(input_device, sizeof input_device, stdin)) {
			input_device[strlen(input_device) - 1] = '\0';	
			if (input_device[0] != '\0')
				device = input_device;
		}
		
		printf("\n");
	}
	
	int port = open(device, O_RDWR | O_NOCTTY);
	if (port < 0) { perror("failed to open"); return -1; }

	// flush previous bytes
	int result = tcflush(port, TCIOFLUSH);
	if (result)	{ perror("tcflush failed");}
	
	// Configuration
	struct termios options;
	
	result = tcgetattr(port, &options);
	if (result) { perror("tcgetattr failed"); close(port); return -2; }

	options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	options.c_oflag &= ~(ONLCR | OCRNL);
	
	options.c_cc[VTIME] = 1;
	options.c_cc[VMIN] = 0;
	
	int choice = 5;
	
	if (advanced_init) {
		printf("Available baud rates are 0) B230400 1) 115200, 2) 57600, 3) 38400, 4) 19200, 5) 9600, 6) 4800, 7) 2400, 8) 1200\n");
		printf("Which one would you like to use (default is 5): ");
		
		char input_baud[8];
		if (fgets(input_baud, sizeof input_baud, stdin)) {
			if (input_baud[0] != '\0')
				choice = atoi(input_baud);
		}
		
		printf("\n");
	}
		
	cfsetospeed(&options, baud_rates[choice]);
	cfsetispeed(&options, baud_rates[choice]);

	result = tcsetattr(port, TCSANOW, &options);
	if (result) { perror("tcsetattr failed"); close(port); return -3; }
	
	return port;
}

