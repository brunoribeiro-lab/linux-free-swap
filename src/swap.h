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
