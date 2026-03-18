#ifndef _COMMON_H
#define _COMMON_H

#define _MUTEX_LOCK(mutex)                              do {} while (0)
#define _MUTEX_UNLOCK(mutex)                              do {} while (0)

#define LOCK()          _MUTEX_LOCK(&random_mutex)
#define UNLOCK()        _MUTEX_UNLOCK(&random_mutex)

void srandom_deterministic(unsigned int x);

#endif
