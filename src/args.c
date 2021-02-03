#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "util.h"

void print_option_detail(options_t *option)
{
    if (strcmp(option->short_param, "") == 0 || strcmp(option->long_param, "") == 0 || strcmp(option->description, "") == 0)
        return;

    int printted =0;
    printted += printf("   %s", option->short_param);

    if(strcmp(option->long_param, option->short_param) != 0){
        printted += printf(", %s", option->long_param);
    }

    printf("%*s",25 - printted, " ");
    printf("%s\n", option->description);
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


void process_args(int start, int length, char *argv[], options_t *options, int options_size)
{
    int idx = start;
    options_t *option = NULL;

    for (; idx < length; idx++)
    {
        if (option != NULL)
            break;

        option = find_options(argv[idx], options_size, options);
    }

    if (option == NULL)
        option = find_options("", options_size, options);

    char x[4];
    char *y = "abcd";

    strcpy(x, y); // warn

    option->command(length, argv, idx);
}
