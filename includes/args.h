#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>
#include <getopt.h>

/* Max size of a file name */
#define FILE_NAME_SIZE 512

/* Defines the command line allowed options struct */
struct options
{
    void (*command)(int, char**, int);
    char* short_param;
    char* long_param;
    char* description;
};

/* Exports options as a global type */
typedef struct options options_t;

/* Public functions section */
void process_args(int argc, char* argv[], int options_size, options_t* options);
void print_option_detail(options_t *option);


#endif
