#ifndef DAEMON_H
#define DAEMON_H

struct daemon_signal {
    void (*sigint_handler)(int);
    void (*sigkill_handler)(int);
    char * pidfile;
};

typedef struct daemon_signal daemon_signal_t;

void daemonize(daemon_signal_t *signal_handler);
void send_daemon_signal(char * pid_path, int signal);

static void save_pid(char*);
static int get_pid(char*);
static void register_signals(daemon_signal_t *signal_handler);

#endif