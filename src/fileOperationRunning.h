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
                    (void)fgets(cmdline, sizeof(cmdline), fp);
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
