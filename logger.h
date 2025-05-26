// safe guard
#ifndef KEY_TRACER
#define KEY_TRACER
#define MAX_SIZE 512
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<linux/input.h>
#include<errno.h>
#include<string.h>



void get_handler(char *pdevice,char *phandler , int verbose);

void connect_handler(char *handler , int verbose);


#endif