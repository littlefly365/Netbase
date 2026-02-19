#ifndef _COMPAT_H
#define _COMPAT_H

#include <stdio.h>

// compat includes
#include "nb_assert.h"
#include "nb_locale.h"
#include "nb_regex.h"
#include "nb_stdlib.h"
#include "nb_time.h"
#include "nb_unistd.h"
#include "tzfile.h"
#include "util.h"
#include "vis.h"
#include  "sys/nb_cdefs_aout.h"
#include  "sys/nb_cdefs.h"
#include  "sys/nb_param.h"
#include  "sys/nb_stat.h"
#include  "sys/nb_time.h"

#ifdef SIGINFO
#else
#define SIGINFO SIGUSR1
#endif

#ifndef __UNCONST
#define __UNCONST(a)   ((void *)(unsigned long)(const void *)(a))
#endif

#ifndef LONG_MAX
#define      LONG_MAX        __LONG_MAX__
#endif

#ifndef ULONG_MAX
#define      ULONG_MAX       (2UL*LONG_MAX+1UL)
#endif

#ifndef SIZE_T_MAX
#define      SIZE_T_MAX      ULONG_MAX
#endif

#if defined(__linux__) || defined(__GNU__) 

static inline char *group_from_gid(unsigned long group, int gid) { return NULL; }
static inline char *user_from_uid(unsigned long user, int uid) { return NULL; }
static inline char *uid_from_user(const char *user, unsigned int *uid) { return NULL; }
static inline int undelete(const char *path) { return 0; }

char *fgetln(FILE *stream, size_t *len);

#define fcpxattr
#define CLK_TCK             0
#define setproctitle             
#define  O_EXLOCK  	0

#endif

#endif
