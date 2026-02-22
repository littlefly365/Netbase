#include <stdio.h>
#include <sys/sysmacros.h>

const char *
devname(dev_t dev, mode_t type)
{
    static char buf[64];

    snprintf(buf, sizeof(buf),
        "%u,%u",
        major(dev),
        minor(dev));

    return buf;
}
