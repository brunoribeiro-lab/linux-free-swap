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
