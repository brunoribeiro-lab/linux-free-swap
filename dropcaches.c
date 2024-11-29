#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>
#include "src/cache/include/cache.h"
#include "src/config/include/config.h"
#include "src/process/include/fileOperationRunning.h"
#include "src/updateSystem/include/updateSystem.h"
#include "src/process/include/processRunning.h"
#include "src/swap/include/swap.h"
#include "src/utils/include/ini.h"

extern bool debug;
extern int cache_ram_threshold, sleep_time;

int main()
{
    printf("Starting...\n");

    // Check if config file exists
    struct stat buffer;
    if (stat(CONFIG_FILE, &buffer) != 0)
    {
        printf("Config not found. Creating default.\n");
        create_default_config();
    }

    // Load config settings
    if (ini_parse(CONFIG_FILE, config_handler, NULL) < 0)
        printf("Error loading config. Using defaults.\n");

    // Display config settings
    printf("Debug: %s\nCache RAM Threshold: %d%%\nSleep time: %d seconds\n", debug ? "Enabled" : "Disabled", cache_ram_threshold, sleep_time);

    openlog("SwapClearance", LOG_PID | LOG_CONS, LOG_USER);

    while (true)
    {
        struct sysinfo info;
        if (sysinfo(&info) == 0)
        {
            if (cache_ram_threshold > 0 && isCacheThresholdExceeded(&info))
            {
                if (debug)
                    syslog(LOG_INFO, "Cache exceeded! %.2f%% > %d%%", (double)(info.bufferram + get_cached_memory()) / info.totalram * 100, cache_ram_threshold);

                if (getuid() == 0)
                {
                    syslog(LOG_INFO, "Clearing cached RAM...");
                    if (system("/bin/echo 3 > /proc/sys/vm/drop_caches") == 0)
                        syslog(LOG_INFO, "Cached RAM cleared!");
                    else syslog(LOG_ERR, "Error clearing cached RAM.");
                }
                else if (debug) syslog(LOG_INFO, "Root required to clear cached RAM.");
            }
            else if (debug) syslog(LOG_INFO, "Cache RAM below threshold.");

            if (info.totalswap > 0 && isSwapInUse())
            {
                if (!isFileOperationRunning() && !processIsRunning("modprobe") && !isSystemUpdating())
                {
                    if (getuid() == 0)
                    {
                        syslog(LOG_INFO, "Clearing swap...");
                        system("swapoff -a && swapon -a");
                        syslog(LOG_INFO, "Swap cleared!");
                    }
                    else if (debug) syslog(LOG_INFO, "Root required to clear swap.");
                }
                else if (debug) syslog(LOG_INFO, "Conditions not met to clear swap.");
            }
            else if (debug) syslog(LOG_INFO, "No swap in use or available.");
        }
        else if (debug) syslog(LOG_ERR, "Error getting system info.");

        sleep(sleep_time);
    }

    closelog();
    return 0;
}
