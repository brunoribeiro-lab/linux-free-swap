#include <stdio.h>
#include <sys/sysinfo.h>
#include "include/cache.h"
#include "../swap/include/swap.h"

extern int cache_ram_threshold;
extern bool debug;

unsigned long get_cached_memory()
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp)
    {
        perror("Error opening /proc/meminfo");
        return 0;
    }

    unsigned long cached = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp))
        if (strncmp(line, "Cached:", 7) == 0)
            sscanf(line, "Cached: %lu kB", &cached);
    fclose(fp);
    return cached * 1024;
}

bool isCacheThresholdExceeded(struct sysinfo *info)
{
    unsigned long cached_ram = info->bufferram + get_cached_memory(), total_ram = info->totalram;
    double cached_percentage = (double)cached_ram / total_ram * 100;

    if (debug)
    {
        printf("Buffered RAM: %lu\nCached RAM: %lu\nTotal RAM: %lu\nCache RAM: %.2f%%\n", info->bufferram, cached_ram, total_ram, cached_percentage);
    }
    return cached_percentage > cache_ram_threshold;
}