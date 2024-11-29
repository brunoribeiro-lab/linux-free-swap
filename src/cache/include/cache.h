#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>
#include <string.h>
#include <sys/sysinfo.h>

unsigned long get_cached_memory();
bool isCacheThresholdExceeded(struct sysinfo *info);

#endif