#include "include/swap.h"
#include <stdio.h>
#include <stdlib.h>

bool isSwapInUse()
{
    FILE *file = fopen("/proc/swaps", "r");
    if (!file)
    {
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] != '\0' && line[0] != '\n')
        {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}
