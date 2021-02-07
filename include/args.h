#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

/* Max size of a file name */
#define FILE_NAME_SIZE 512

/* Defines the command line allowed options struct */
typedef struct options
{
    void (*command)(int, char**, int);
    char* short_param;
    char* long_param;
    char* description;
} options_t;

/* Public functions section */
void process_args(int start, int length, char *argv[], options_t *options, int options_size);
void print_option_detail(options_t *option);


#endif
