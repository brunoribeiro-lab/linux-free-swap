#include "include/fileOperationRunning.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifndef DT_DIR
#define DT_DIR 4
#endif

bool isFileOperationRunning()
{
    DIR *dir = opendir("/proc");
    if (dir == NULL)
        return false;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
        {
            char path[1024];
            snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);
            FILE *fp = fopen(path, "r");
            if (fp)
            {
                char process_name[256];
                if (fgets(process_name, sizeof(process_name), fp))
                {
                    if (strstr(process_name, "cp") || strstr(process_name, "mv") ||
                        strstr(process_name, "tar") || strstr(process_name, "zip"))
                    {
                        fclose(fp);
                        closedir(dir);
                        return true;
                    }
                }
                fclose(fp);
            }
        }
    }
    closedir(dir);
    return false;
}
