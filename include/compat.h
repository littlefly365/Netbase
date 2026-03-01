#ifndef _COMPAT_H
#define _COMPAT_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/acl.h>

#ifdef SIGINFO
#else
#define SIGINFO SIGUSR1
#endif

#ifdef SIZE_T_MAX
#else
#define SIZE_T_MAX SIZE_MAX
#endif

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
    (void)h;
    if (b) *b = 512;
    return "512";
}

static inline char *
flags_to_string(u_long f, char *d)
{
    (void)f;
    return d;
}

static inline u_long
*string_to_flags(char **f, u_long *d, char *N)
{
    (void)f;
    (void)N;
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
#define SF_IMMUTABLE	0
#define SF_APPEND	0
#define UF_IMMUTABLE	0
#define UF_APPEND	0

// fstypes.h
#define     MNT_LOCAL       0x00001000
#define      MNT_RDONLY      0x00000001

// fnmatch.h
#define      FNM_CASEFOLD    0x08

#endif
