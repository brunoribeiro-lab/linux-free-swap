#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

extern bool debug;
extern int cache_ram_threshold, sleep_time;

#define CONFIG_FILE "/etc/dropcache.conf"

extern void create_default_config();
extern int config_handler(void *user, const char *section, const char *name, const char *value);

#endif