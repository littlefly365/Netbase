#define _GNU_SOURCE
#include <errno.h>
#include <unistd.h>

int fchflags(int fd, unsigned long flags)
{
    (void)fd;
    (void)flags;

    errno = EOPNOTSUPP;
    return -1;
}
