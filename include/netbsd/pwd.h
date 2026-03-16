#ifndef _NB_PWD_H
#define _NB_PWD_H

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

static char *user_from_uid(uid_t uid, int nouser) 
{ 
    struct passwd *pw;

    pw = getpwuid(uid);
    if (pw)
        return pw->pw_name;

    static char buf[32];
    snprintf(buf, sizeof(buf), "%u", (unsigned)uid);
    return buf;
}
static char *group_from_gid(gid_t gid, int nogroup) 
{ 
    struct group *gr;

    gr = getgrgid(gid);
    if (gr)
        return gr->gr_name;

    static char buf[32];
    snprintf(buf, sizeof(buf), "%u", (unsigned)gid);
    return buf;
}

static int
uid_from_user(const char *name, uid_t *uid)
{
    struct passwd *pw;

    pw = getpwnam(name);
    if (pw) {
        *uid = pw->pw_uid;
        return 0;
    }

    char *end;
    long val = strtol(name, &end, 10);

    if (*end == '\0') {
        *uid = (uid_t)val;
        return 0;
    }

    return -1;
}

static int
gid_from_group(const char *name, gid_t *gid)
{
    struct group *gr;

    gr = getgrnam(name);
    if (gr) {
        *gid = gr->gr_gid;
        return 0;
    }

    char *end;
    long val = strtol(name, &end, 10);

    if (*end == '\0') {
        *gid = (gid_t)val;
        return 0;
    }

    return -1;
}

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

#define      _PASSWORD_LEN           128

#endif
