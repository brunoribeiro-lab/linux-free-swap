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
