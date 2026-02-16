#ifndef _COMPAT_H
#define _COMPAT_H

#include <stddef.h>
#include <stdint.h>  
#include <inttypes.h> 
#include <time.h> 
#include <locale.h> 

#include <sys/types.h> 

//stdlib.h
extern const char *__progname;
const char *getprogname(void);
void setprogname(const char *name);
int reallocarr(void *ptrp, size_t number, size_t size);
double		strtod_l(const char * __restrict, char ** __restrict, locale_t);

#define	HN_AUTOSCALE		0x20
#define	HN_GETSCALE		0x10
#define	HN_DIVISOR_1000		0x08
#define	HN_B			0x04
#define	HN_NOSPACE		0x02
#define	HN_DECIMAL		0x01

// unistd.h
void *setmode(const char *mode_str);
mode_t getmode(const void *set, mode_t mode);
void strmode(mode_t mode, char *bp);

// time.h
static inline time_t mktime_z(void *tz, struct tm *tm) {
    (void)tz;  
    return mktime(tm);
}

// assert.h
#define _DIAGASSERT(e) 

// locale.h
static struct __locale_struct _lc_C_locale;
#define LC_C_LOCALE		((locale_t)__UNCONST(&_lc_C_locale))

// sys/cdefs.h
#define __COPYRIGHT(_s)
#define __RCSID(_s)
#define	__predict_false(exp)	__builtin_expect((exp) != 0, 0)
#define __unused 
#define __used __unused
#define __dead __attribute__((__noreturn__))

// sys/stat.h
#define	S_ISTXT	0001000
#define	_S_IFWHT  0160000
#define	S_IFWHT  _S_IFWHT
#define	S_ISWHT(m)	(((m) & _S_IFMT) == _S_IFWHT)
#define	_S_IFMT	  0170000
#define	S_IFMT	 _S_IFMT

// sys/time.h
#define timespecadd(tsp, usp, vsp)                                      \
        do {                                                            \
                (vsp)->tv_sec = (tsp)->tv_sec + (usp)->tv_sec;          \
                (vsp)->tv_nsec = (tsp)->tv_nsec + (usp)->tv_nsec;       \
                if ((vsp)->tv_nsec >= 1000000000L) {                    \
                        (vsp)->tv_sec++;                                \
                        (vsp)->tv_nsec -= 1000000000L;                  \
                }                                                       \
        } while (/* CONSTCOND */ 0)
#define timespecsub(tsp, usp, vsp)                                      \
        do {                                                            \
                (vsp)->tv_sec = (tsp)->tv_sec - (usp)->tv_sec;          \
                (vsp)->tv_nsec = (tsp)->tv_nsec - (usp)->tv_nsec;       \
                if ((vsp)->tv_nsec < 0) {                               \
                        (vsp)->tv_sec--;                                \
                        (vsp)->tv_nsec += 1000000000L;                  \
                }                                                       \
        } while (/* CONSTCOND */ 0)

// custom
static inline char *group_from_gid(unsigned long group, int gid) { return NULL; }
static inline char *user_from_uid(unsigned long user, int uid) { return NULL; }
static inline int undelete(const char *path) { return 0; }

#define SIGINFO SIGUSR1
#define __UNCONST(a)   ((void *)(unsigned long)(const void *)(a))
#define fcpxattr
#endif
