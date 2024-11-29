#include "include/updateSystem.h"
#include <stdio.h>
#include <stdlib.h>

bool isSystemUpdating()
{
    return system("pgrep -x apt > /dev/null || pgrep -x dnf > /dev/null || pgrep -x yum > /dev/null || pgrep -x zypper > /dev/null") == 0;
}
