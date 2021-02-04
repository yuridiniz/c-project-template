#ifndef DAEMON_H
#define DAEMON_H

struct daemon_signal {
    void (*sigint_handler)(int);
    void (*sigkill_handler)(int);
};

typedef struct daemon_signal daemon_signal_t;

void daemonize(daemon_signal_t *signal_handler);
int send_daemon_signal(int pid, int signal);
int daemon_is_running(int pid);

#endif