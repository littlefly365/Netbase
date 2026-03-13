#ifndef _SYS_EVENT_H
#define _SYS_EVENT_H

#include <sys/epoll.h>
#include <stdint.h>

struct kevent {
    uintptr_t ident;
    short filter;
    unsigned short flags;
    unsigned int fflags;
    intptr_t data;
    void *udata;
};

static inline int 
kqueue(void){
return 0;
}
static inline int 
kevent(int, const struct kevent *, int, struct kevent *, int, const struct timespec *){
return 0;
}

#define EV_SET(kevp,a,b,c,d,e,f) do { \
    (kevp)->ident = (a); \
    (kevp)->filter = (b); \
    (kevp)->flags = (c); \
    (kevp)->fflags = (d); \
    (kevp)->data = (e); \
    (kevp)->udata = (f); \
} while(0)

#define EV_ADD 0x0001
#define EV_ENABLE 0x0004
#define EV_CLEAR 0x0020

#define EVFILT_READ -1
#define EVFILT_VNODE -2

#define NOTE_DELETE 0x0001
#define NOTE_RENAME 0x0002

#endif
