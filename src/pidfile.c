#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#include "pidfile.h"

int get_pid(char *pidfile)
{
    int pid = 0;
    FILE *f;

    if (!(f = fopen(pidfile, "r")))
        return 0;

    fscanf(f, "%d", &pid);
    fclose(f);

    return pid;
}

int save_pid(char *pidfile)
{
    FILE *f = fopen(pidfile, "w+");

    if(f == NULL) {
        printf("%d\n", errno);
        printf("%s\n", strerror(errno));
        return -1;
    }

    int pid = (int)getpid();

    fprintf(f, "%d", pid);
    fclose(f);

    return pid;
}
