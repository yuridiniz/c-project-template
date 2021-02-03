#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "util.h"

void print_option_detail(options_t *option)
{
    if (strcmp(option->short_param, "") == 0 || strcmp(option->long_param, "") == 0 || strcmp(option->description, "") == 0)
        return;

    printf("   %s,%s   %s\n", option->short_param, option->long_param, option->description);
}


options_t *find_options(char *str, int options_size, options_t *options)
{
    options_t *option = NULL;

    for (int opt = 0; opt < options_size; opt++)
    {
        options_t val = (options[opt]);
        if (strcmp(str, val.short_param) == 0 || strcmp(str, val.long_param) == 0)
        {
            option = &val;
            break;
        }
    }

    return option;
}


void process_args(int argc, char *argv[], int options_size, options_t *options)
{
    int idx = -1;
    options_t *option = NULL;

    for (idx = 0; idx < argc; idx++)
    {
        if (option != NULL)
            break;

        option = find_options(argv[idx], options_size, options);
    }

    if (option == NULL)
        option = find_options("", options_size, options);

    if (option != NULL)
        option->command(argc, argv, idx);
}
