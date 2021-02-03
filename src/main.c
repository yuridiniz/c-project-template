#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "args.h"
#include "util.h"
#include "daemon.h"

void show_help_command(int argc, char *argv[], int index);
void start_command(int argc, char *argv[], int index);
void stop_command(int argc, char *argv[], int index);

static options_t options[] = {
    {start_command, "start", "start", "Iniciar o serviço"},
    {stop_command, "stop", "stop", "Para o serviço"},
    {show_help_command, "-h", "--help", "Exibe os detalhes das operações"},
    {show_help_command, "", "", ""},
};

void stop_command(int argc, char *argv[], int index) {
    send_daemon_signal("/var/run/mqttd.pid", SIGKILL);
    exit(0);
}

void sigkill_handler(int _) {
    exit(1);
}

void start_command(int argc, char *argv[], int index) {
    daemon_signal_t config;
    config.pidfile = "/var/run/mqttd.pid";
    config.sigkill_handler = sigkill_handler;

    daemonize(&config);

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
