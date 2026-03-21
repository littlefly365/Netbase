#include <stdlib.h>
#include <errno.h>
#include "nb_time.h"

#define FREE_PRESERVES_ERRNO 1

void
tzfree(timezone_t sp)
{
  int err;
  if (!FREE_PRESERVES_ERRNO)
    err = errno;
  free(sp);
  if (!FREE_PRESERVES_ERRNO)
    errno = err;
}
