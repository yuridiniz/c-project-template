#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "args.h"
#include "util.h"
#include "daemon.h"
#include "pidfile.h"

void show_help_command(int argc, char *argv[], int index);
void start_command(int argc, char *argv[], int index);
void stop_command(int argc, char *argv[], int index);

static options_t options[] = {
    {start_command,         "start",    "start", "  Iniciar o serviço"},
    {stop_command,          "stop",     "stop",     "Para o serviço"},
    {show_help_command,     "-h",       "--help",   "Exibe os detalhes das operações"},
    {show_help_command,     "",         "",         ""},
};

void sigkill_handler(int _) {
    exit(0);
}

void check_deamon_to_start(char * pid_file) {
    int pid = get_pid(pid_file);

    if(access(pid_file, W_OK) != 0 && errno == EACCES) {
        printf("[ERROR] Não foi possível iniciar o processo deamon devido ao mesmo ter sido iniciado anteriormente por outro usuário, pare o proceso anterior usando o mesmo usuário que iniciou\n");
        exit(EXIT_FAILURE);
    }

    if(daemon_is_running(pid)) {
        printf("[WARN] Processo já está em execução com pid %d\n", pid);
        exit(EXIT_FAILURE);
    }
}

int check_daemon_to_stop(char * pid_file) 
{
    int pid = get_pid(pid_file);

    if(access(pid_file, F_OK) != 0 && errno == EACCES) {
        printf("[ERROR] Não foi possível parar o processo de pid %d devido ao mesmo ter sido iniciado por outro usuário\n", pid);
        exit(EXIT_FAILURE);
    }

    if(!daemon_is_running(pid)) {
        printf("[WAN] Processo não está em execução\n");
        exit(EXIT_FAILURE);
    }
    
    return pid;
}

void stop_command(int argc, char *argv[], int index) {
    char * pid_file = "/var/run/mqttd/mqttd.pid";
    
    int pid = check_daemon_to_stop(pid_file);
    send_daemon_signal(pid, SIGKILL);
    
    remove(pid_file);
    exit(0);
}

void start_command(int argc, char *argv[], int index) {
    char * pid_file = "/var/run/mqttd/mqttd.pid";

    check_deamon_to_start(pid_file);
    
    daemon_signal_t config;
    config.sigkill_handler = sigkill_handler;

    daemonize(&config);

    save_pid(pid_file);

    pause();
}

void show_help_command(int argc, char *argv[], int index)
{
    UNUSED(argv);
    UNUSED(argc);

    printf("A initial template project for C projects.\n\n");
    printf("Usage: %s [options]\n\n", argv[0]);
    printf("Options:\n");

    for (int i = 0; i < (int) LEN(options); i++)
    {
        options_t option = options[i];
        print_option_detail(&option);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    process_args(1, argc, argv, options, (int) LEN(options));

    return EXIT_SUCCESS;
}
