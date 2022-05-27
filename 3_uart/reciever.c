#include "common.h"


void do_something(int port){
	
	char buffer[1];
	
	while(1) {
		int bytes_read = read(port, &buffer, sizeof(buffer));
		
		if (bytes_read > 0) {
			if(buffer[0] == '.') break;
			
			printf("%s", buffer);
			fflush(stdout); 
		}		
	}	
    
    printf("\n");
}


int main(int argc, char *argv[])
{
	int advanced_init = (argc > 1 && !strcmp(argv[1],"-adv"));
	
	if (argc > 1 && strcmp(argv[1],"-adv")) {
		printf("Incorrect args, to start advanced mode use -adv\n");
		return(1);
	}

	int port = port_init(advanced_init, "/dev/ttyUSB1");
	if (port < 0) {
		printf("Failed to initialize port");
		return(2);
	}
	
	do_something(port);

	close(port);

}
