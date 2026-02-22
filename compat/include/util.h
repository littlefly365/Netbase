/*      $NetBSD: util.h,v 1.69 2016/04/10 19:05:50 roy Exp $    */

/*-
 * Copyright (c) 1995
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _UTIL_H
#define _UTIL_H

char *strptime(const char *buf, const char *fmt, struct tm *tm);
void		logwtmp(const char *, const char *, const char *);
time_t parsedate(const char *datestr, const time_t *time, const int *tzoff);
size_t estrlcpy(char *dst, const char *src, size_t len);
size_t estrlcat(char *dst, const char *src, size_t len);
char *estrdup(const char *s);
char *estrndup(const char *s, size_t len);
void *emalloc(size_t n);
void *ecalloc(size_t n, size_t s);
void *erealloc(void *p, size_t n);
void ereallocarr(void *p, size_t n, size_t s);
FILE *efopen(const char *p, const char *m);
int easprintf(char ** __restrict ret, const char * __restrict format, ...);
int evasprintf(char ** __restrict ret, const char * __restrict format, va_list ap);
intmax_t estrtoi(const char * nptr, int base, intmax_t lo, intmax_t hi);
uintmax_t estrtou(const char * nptr, int base, uintmax_t lo, uintmax_t hi);

#endif
