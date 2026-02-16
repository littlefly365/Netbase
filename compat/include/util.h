#ifndef _UTIL_H
#define _UTIL_H

char *strptime(const char *buf, const char *fmt, struct tm *tm);
void		logwtmp(const char *, const char *, const char *);
time_t parsedate(const char *datestr, const time_t *time, const int *tzoff);
#endif
