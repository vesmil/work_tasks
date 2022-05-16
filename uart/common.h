#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int port_init(int advanced_init, char* device);

#endif
