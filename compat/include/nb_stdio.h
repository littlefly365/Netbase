#ifndef _NB_STDIO_H
#define _NB_STDIO_H

char *fgetln(FILE *stream, size_t *len);
wchar_t *fgetwln(FILE *stream, size_t *lenp);

#endif
