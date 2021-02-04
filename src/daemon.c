#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "daemon.h"
#include "pidfile.h"

static void register_signals(daemon_signal_t *signal_handler);

void daemonize(daemon_signal_t *signal_handler)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        pid = fork();
        if (pid > 0)
            exit(EXIT_SUCCESS);
    }

    if (pid < 0)
    {
        printf("Não foi possível iniciar o processo daemon");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    register_signals(signal_handler);
}

int send_daemon_signal(int pid, int signal)
{
    return kill(pid, signal);
}

int daemon_is_running(int pid) {
    if(pid <= 0)
        return 0;
    
    int result = send_daemon_signal(pid, 0);
    return (result < 0) ? 0 : 1;
}

static void register_signals(daemon_signal_t *signal_handler)
{
    if(signal_handler == NULL)
        return;
    
    signal(SIGINT, signal_handler->sigint_handler);
    signal(SIGKILL, signal_handler->sigkill_handler);
}
