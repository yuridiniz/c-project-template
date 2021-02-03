#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "daemon.h"

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

    save_pid(signal_handler->pidfile);
}

void send_daemon_signal(char * path, int signal)
{
    int pid = get_pid(path);
    if(pid == 0)
        return;
        
    kill(pid, signal);
}

void register_signals(daemon_signal_t *signal_handler)
{
    if(signal_handler == NULL)
        return;
    
    signal(SIGINT, signal_handler->sigint_handler);
    signal(SIGKILL, signal_handler->sigkill_handler);
}

int get_pid(char * pidfile)
{
    int pid = 0;
    FILE *f;

    if (!(f=fopen(pidfile,"r")))
        return 0;

    fscanf(f, "%d", &pid);
    fclose(f);

    return pid;
}

void save_pid(char * pidfile)
{
    FILE *f = fopen(pidfile, "w+");
    fprintf(f, "%d", (int)getpid());
    fclose(f);
}

