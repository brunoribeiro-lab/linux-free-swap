#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <syslog.h>
#include <ctype.h>

// Function to check if a process related to file operations is running
bool isFileOperationRunning() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir("/proc");
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_LNK && strcmp(entry->d_name, "self") != 0) {
                char path[512];
                snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);
                FILE *fp = fopen(path, "r");
                if (fp != NULL) {
                    char cmdline[512];
                    fgets(cmdline, sizeof(cmdline), fp);
                    fclose(fp);

                    // Check for file operations
                    if (strstr(cmdline, "cp") != NULL || strstr(cmdline, "mv") != NULL ||
                        strstr(cmdline, "tar") != NULL || strstr(cmdline, "zip") != NULL) {
                        closedir(dir);
                        return true;
                    }
                }
            }
        }
        closedir(dir);
    }
    return false;
}

// Function to check if the system is performing a package update
bool isSystemUpdating() {
    FILE *fp = popen("pgrep -x apt || pgrep -x dnf || pgrep -x yum || pgrep -x zypper", "r");
    if (fp != NULL) {
        char output[16];
        if (fgets(output, sizeof(output), fp) != NULL) {
            pclose(fp);
            return true;
        }
        pclose(fp);
    }
    return false;
}

// Function to check if a process is running
bool processIsRunning(const char *process) {
    char command[128];
    snprintf(command, sizeof(command), "pgrep -x %s", process);
    FILE *fp = popen(command, "r");
    char output[16];
    fgets(output, sizeof(output), fp);
    pclose(fp);

    // Remove non-numeric characters from the output
    for (int i = 0; i < strlen(output); i++) {
        if (!isdigit(output[i])) {
            output[i] = '\0';
            break;
        }
    }

    // Check if process is not running
    if (strlen(output) == 0) {
        return false;
    }

    return true;
}

// Function to check if the swap space is actively in use
bool isSwapInUse() {
    FILE *fp = fopen("/proc/swaps", "r");
    if (fp != NULL) {
        char line[256];
        // Skip the header line
        fgets(line, sizeof(line), fp);

        // Check if there are any entries indicating swap usage
        while (fgets(line, sizeof(line), fp) != NULL) {
            char devname[64];
            char swapfile[64];
            char type[16];
            int used;

            sscanf(line, "%s %s %s %d", devname, swapfile, type, &used);
            if (used > 0) {
                fclose(fp);
                return true;
            }
        }

        fclose(fp);
    }
    return false;
}

int main() {
    // Open syslog with desired options
    openlog("SwapClearance", LOG_PID | LOG_CONS, LOG_USER);

    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        syslog(LOG_INFO, "Free RAM: %lu bytes", info.freeram);
        syslog(LOG_INFO, "Buffered RAM: %lu bytes", info.bufferram);

        if (info.totalswap > 0) {
            syslog(LOG_INFO, "Swap: %lu bytes", info.totalswap);

            // Check if swap is in use
            if (isSwapInUse()) {
                // Check if file operations or system update is running
                if (isFileOperationRunning()) {
                    syslog(LOG_INFO, "File operations. Skipping swap clearance.");
                } else if (processIsRunning("modprobe")) {
                    syslog(LOG_INFO, "Steam Vulkan shader caching. Skipping swap clearance.");
                }  else if (isSystemUpdating()) {
                    syslog(LOG_INFO, "System update in progress. Skipping swap clearance.");
                } else {
                    // Free swap
                    if (getuid() == 0) {
                        syslog(LOG_INFO, "Clearing swap...");
                        system("/bin/echo 3 > /proc/sys/vm/drop_caches && swapoff -a && swapon -a");
                        syslog(LOG_INFO, "Swap cleared!");
                    } else {
                        syslog(LOG_INFO, "You must have root privileges to clear the swap.");
                    }
                }
            } else {
                syslog(LOG_INFO, "No swap space in use.");
            }
        } else {
            syslog(LOG_INFO, "No swap space available.");
        }
    } else {
        syslog(LOG_ERR, "Error getting system information.");
    }

    // Close syslog
    closelog();

    return 0;
}
