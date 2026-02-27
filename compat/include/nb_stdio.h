#ifndef _NB_STDIO_H
#define _NB_STDIO_H

#include <stdarg.h>

char *fgetln(FILE *stream, size_t *len);
wchar_t *fgetwln(FILE *stream, size_t *lenp);
int     snprintf_ss(char *restrict, size_t, const char * __restrict, ...);
int     vsnprintf_ss(char *restrict, size_t, const char * __restrict, va_list);
#endif
