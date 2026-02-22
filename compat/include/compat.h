#ifndef _COMPAT_H
#define _COMPAT_H

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/acl.h>

// compat includes
#include "nb_assert.h"
#include "nb_locale.h"
#include "nb_regex.h"
#include "nb_stdlib.h"
#include "nb_stdio.h"
#include "nb_time.h"
#include "nb_unistd.h"
#include "tzfile.h"
#include "util.h"
#include  "sys/nb_cdefs.h"
#include  "sys/nb_param.h"
#include  "sys/nb_stat.h"
#include  "sys/nb_time.h"

#ifndef __BEGIN_DECLS

#ifdef __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS   }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

#endif

#include "vis.h"

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

static inline char *group_from_gid(unsigned long group, int gid) { return NULL; }
static inline char *user_from_uid(unsigned long user, int uid) { return NULL; }
static inline char *uid_from_user(const char *user, unsigned int *uid) { return NULL; }
static inline int undelete(const char *path) { return 0; }

#define fcpxattr
#define CLK_TCK             0
#define setproctitle             
#define  O_EXLOCK  	0

#ifndef REG_STARTEND
#define REG_STARTEND  0x0080
#endif
#ifndef NL_TEXTMAX
#define NL_TEXTMAX          2048
#endif

#define chflags 
typedef unsigned long u_long;
typedef unsigned int u_int;
#ifndef S_BLKSIZE
#define S_BLKSIZE 512
#endif

static inline const char *
getbsize(int *h, long *b)
{
    if (b) *b = 512;
    return "512";
}

static inline char *
flags_to_string(u_long f, char *d)
{
    (void)f;
    return d;
}

typedef int32_t __devmajor_t, __devminor_t;
#define devmajor_t __devmajor_t
#define devminor_t __devminor_t
#define NODEVMAJOR (-1)
#define major(x)        ((devmajor_t)(((uint32_t)(x) & 0x000fff00) >>  8))
#define minor(x)        ((devminor_t)((((uint32_t)(x) & 0xfff00000) >> 12) | \
                                   (((uint32_t)(x) & 0x000000ff) >>  0)))

static inline long
lpathconf(const char *path, int name)
{
    (void)path;
    (void)name;
    return -1;
}

#define _PC_ACL_EXTENDED	0
#define ACL_TYPE_NFS4		0
#define _PC_ACL_NFS4		0

typedef struct __acl_ext *acl_t;

static inline acl_t
acl_get_link_np(const char *path, acl_type_t type)
{
    (void)path;
    (void)type;
    return NULL;
}

static inline int
acl_set_link_np(const char *path, acl_type_t type, acl_t name)
{
    (void)path;
    (void)type;
    (void)name;
    return 0;
}

static inline acl_t
acl_get_fd_np(int type, int name)
{
    (void)type;
    (void)name;
    return 0;
}

static inline acl_t
acl_set_fd_np(int path, acl_t acl, int name)
{
    (void)path;
    (void)acl;
    (void)name;
    return NULL;
}

static inline int
acl_is_trivial_np(acl_t acl, int *trivial)
{
    (void)acl;
    if (trivial)
        *trivial = 1;
    return 0;
}
#define st_atimespec st_atim
#define st_mtimespec st_mtim
#define st_flags st_mode

#ifndef AT_FDCWD
#define     AT_FDCWD                -100
#endif

#ifndef NODEV
#define NODEV ((dev_t)-1)
#endif

#ifndef _EXT
#define _EXT(fp) ((struct __sfileext *)(void *)((fp)->_ext._base))
#endif

static int debug;
#endif
