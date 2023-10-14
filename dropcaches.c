#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <syslog.h>
#include <ctype.h>
#include "src/updateSystem.h"
#include "src/fileOperationRunning.h"
#include "src/processRunning.h"
#include "src/swap.h"

int main() {
    // Open syslog with desired options
    openlog("SwapClearance", LOG_PID | LOG_CONS, LOG_USER);
    bool debug = false;
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        if(debug){
            syslog(LOG_INFO, "Free RAM: %lu bytes", info.freeram);
            syslog(LOG_INFO, "Buffered RAM: %lu bytes", info.bufferram);
        }
        if (info.totalswap > 0) {
            if(debug){
                syslog(LOG_INFO, "Swap: %lu bytes", info.totalswap);
            }
            // Check if swap is in use
            if (isSwapInUse()) {
                // Check if file operations or system update is running
                if (isFileOperationRunning()) {
                    if(debug){
                        syslog(LOG_INFO, "File operations. Skipping swap clearance.");
                    }
                } else if (processIsRunning("modprobe")) {
                    if(debug){
                        syslog(LOG_INFO, "Steam Vulkan shader caching. Skipping swap clearance.");
                    }
                }  else if (isSystemUpdating()) {
                    if(debug){
                        syslog(LOG_INFO, "System update in progress. Skipping swap clearance.");
                    }
                } else {
                    // Free swap
                    if (getuid() == 0) {
                        if(!debug){
                            syslog(LOG_INFO, "Free RAM: %lu bytes", info.freeram);
                            syslog(LOG_INFO, "Buffered RAM: %lu bytes", info.bufferram);
                        }
                        syslog(LOG_INFO, "Clearing swap...");
                        system("/bin/echo 3 > /proc/sys/vm/drop_caches && swapoff -a && swapon -a");
                        syslog(LOG_INFO, "Swap cleared!");
                    } else if(debug) {
                        syslog(LOG_INFO, "You must have root privileges to clear the swap.");
                    }
                }
            } else if(debug) {
                syslog(LOG_INFO, "No swap space in use.");
            }
        } else if(debug) {
            syslog(LOG_INFO, "No swap space available.");
        }
    } else if(debug) {
        syslog(LOG_ERR, "Error getting system information.");
    }

    // Close syslog
    closelog();

    return 0;
}
