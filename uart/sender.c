#include "common.h"

void do_something(int port){

	char inp[1];
	
    system("/bin/stty raw");
	
	while((inp[0] = getchar()) != '.') {		
		write(port, inp, sizeof(inp));	
	}
	
	write(port, inp, sizeof(inp));	
	
    system("/bin/stty cooked");
    
    printf("\n");
}


int main(int argc, char *argv[])
{
	int advanced_init = (argc > 1 && !strcmp(argv[1],"-adv"));
	
	if (argc > 1 && strcmp(argv[1],"-adv")) {
		printf("Incorrect args, to start advanced mode use -adv\n");
		return(1);
	}

	int port = port_init(advanced_init, "/dev/ttyUSB0");
	if (port < 0) {
		printf("Failed to initialize port");
		return(2);
	}

	do_something(port);

	close(port);
}
