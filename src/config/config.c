#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "../utils/include/ini.h"
#include "../config/include/config.h"

bool debug = false;
int cache_ram_threshold = 50, sleep_time = 5;

#define CONFIG_FILE "/etc/dropcache.conf"

void create_default_config()
{
    FILE *file = fopen(CONFIG_FILE, "w");
    if (file)
    {
        fprintf(file, "[Settings]\ndebug = %d\ncache_ram_threshold = %d\nsleep_time = %d\n", debug, cache_ram_threshold, sleep_time);
        fclose(file);
        printf("Config created at %s.\n", CONFIG_FILE);
    }
    else
    {
        perror("Error creating config file");
        exit(EXIT_FAILURE);
    }
}

int config_handler(void *user, const char *section, const char *name, const char *value)
{
    if (strcmp(section, "Settings") == 0)
    {
        if (strcmp(name, "debug") == 0)
            debug = atoi(value);
        else if (strcmp(name, "cache_ram_threshold") == 0)
            cache_ram_threshold = atoi(value);
        else if (strcmp(name, "sleep_time") == 0)
            sleep_time = atoi(value);
    }
    return 1;
}