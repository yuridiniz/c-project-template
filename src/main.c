#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "util.h"

void show_help(int argc, char *argv[], int index);

static options_t options[] = {
    {show_help, "-h", "--help", "Exibe os detalhes das operações"},
    {show_help, "", "", ""},
};

void show_help(int argc, char *argv[], int index)
{
    UNUSED(argv);
    UNUSED(argc);

    printf("Usage: %s \n\n", "mqttd");

    for (int i = 0; i < (int) LEN(options); i++)
    {
        options_t option = options[i];
        print_option_detail(&option);
    }
}

int main(int argc, char *argv[])
{
    process_args(argc, argv, (int) LEN(options), options);
    return EXIT_SUCCESS;
}
