#include "include/processRunning.h"
#include <stdio.h>
#include <stdlib.h>

bool processIsRunning(const char *process)
{
    char command[512];
    snprintf(command, sizeof(command), "pgrep -x %s > /dev/null", process);
    return system(command) == 0;
}