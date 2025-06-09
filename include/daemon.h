#ifndef DAEMON_H
#define DAEMON_H
#define PID_FILE "/var/run/keytrace.pid"




void daemonize();
void handle_signal(int sig);
int is_already_running();
#endif
