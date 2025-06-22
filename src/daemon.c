#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include "../include/daemon.h"
#include "../include/logger.h"
#include "../include/config.h"

extern pid_t sender_pid;


/** 
* checks the process is exit or not
*
* @return 1 if process exit or 0 for no process found.
*/
int is_already_running() {
    FILE *pid_fp = fopen(PID_FILE, "r");
    if (pid_fp) {
        pid_t pid;
        if (fscanf(pid_fp, "%d", &pid) == 1) {
            fclose(pid_fp);
            if (kill(pid, 0) == 0 || errno == EPERM) {
                // Process is running or we don't have permission
                return 1;
            } else if (errno == ESRCH) {
                // Stale PID file, process doesn't exist
                unlink(PID_FILE);
            }
        } else {
            fclose(pid_fp);
        }
    }
    return 0;
}
 
/**
* receive the registered signals and handle it
* 
* @parm sig - Type of signal
*/
void handle_signal(int sig) {
    
    if(sender_pid > 0){
            kill(sender_pid ,SIGTERM);
    }

    switch (sig) {
        case SIGTERM:
        case SIGINT:
            syslog(LOG_NOTICE, "Daemon terminated.");
            closelog();
            remove(PID_FILE);
            exit(EXIT_SUCCESS);
        case SIGHUP:
            syslog(LOG_INFO, "Received SIGHUP (can reload config here).");
            break;
    }
}


//Writes PID to file

void write_pid(){
    FILE *fp = fopen(PID_FILE, "w");
    if (fp) {
        fprintf(fp, "%d", getpid());
        fclose(fp);
    } else {
        syslog(LOG_ERR, "Failed to write PID file.");
        exit(EXIT_FAILURE);
    }
}

void generate_log_file(){
    if(access(LOG_PATH,F_OK) == 0){
        return;
    }
    if(mkdir("/var/tmp/.syslog/",0777) == 0){
        return;
    }else{
        syslog(LOG_ERR,"Fail to create the directory for log");
        exit(EXIT_FAILURE);
    }
}

/**
 * Daemonize the process and close all 
 * input 
 * output
 * error pipelines and redirect to null channel 
 * for log check tail -f /var/log/syslog
 */

void daemonize() {
    pid_t pid;

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS); // parent exits

    if (setsid() < 0) exit(EXIT_FAILURE);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS); // session leader exits

    chdir("/");
    umask(0022);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    signal(SIGHUP, handle_signal);

    openlog("keytrace", LOG_PID, LOG_DAEMON);
    write_pid();
    generate_log_file();
    syslog(LOG_INFO,"Daemon started successfully");
}
