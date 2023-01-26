#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        printf("Free: %lu bytes\n", info.freeram);
        printf("Buffe: %lu bytes\n", info.bufferram);
        if(info.totalswap > 0){
            printf("Swap: %lu bytes\n", info.totalswap);
            // free swap
            if (getuid() == 0) {
                system("/bin/echo 3 > /proc/sys/vm/drop_caches && swapoff -a && swapon -a");
                printf("Swap cleared!\n");
            } else {
                printf("You must have root privileges to clear the swap.\n");
            }
        }else{
            printf("Nothing to do\n");
        }
    } else {
        printf("Error getting system information\n");
    }
    return 0;
}
