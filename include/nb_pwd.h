#ifndef _NB_PWD_H
#define _NB_PWD_H

#include <stdlib.h>
#include <pwd.h>

static inline char *group_from_gid(unsigned long group, int gid) { return NULL; }
static inline char *user_from_uid(unsigned long user, int uid) { return NULL; }
static inline int gid_from_group(const char *user, unsigned int *uid) { return 0; }
static inline int uid_from_user(const char *user, unsigned int *uid) { return 0; }
static inline int undelete(const char *path) { return 0; }

static inline int
pwcache_userdb(int (*a)(int), void (*b)(void), struct passwd *(c)(const char *), struct passwd *(*d)(uid_t))
{
        return 0;
}

static inline int
pwcache_groupdb(int (*a)(int), void (*b)(void), struct group *(*c)(const char *), struct group *(*d)(gid_t))
{
        return 0;
}

#endif
