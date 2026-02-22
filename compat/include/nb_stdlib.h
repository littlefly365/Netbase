/*      $NetBSD: stdlib.h,v 1.125.2.2 2024/10/13 10:39:53 martin Exp $  */

/*-
 * Copyright (c) 1990, 1993
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
 *
 *      @(#)stdlib.h    8.5 (Berkeley) 5/19/95
 */

#ifndef _NB_STDLIB_H
#define _NB_STDLIB_H

#include <stddef.h>
#include <locale.h>
#include <stdint.h>

extern char *__progname;
const char *getprogname(void);
void setprogname(char *name);
int reallocarr(void *ptrp, size_t number, size_t size);
double		strtod_l(const char * __restrict, char ** __restrict, locale_t);
int humanize_number(char *buf, size_t len, int64_t bytes, const char *suffix, int scale, int flags);
intmax_t strtoi(const char *__restrict nptr, char **__restrict endptr, int base, intmax_t lo, intmax_t hi, int *rstatus);
uintmax_t strtou(const char *__restrict nptr, char **__restrict endptr, int base, uintmax_t lo, uintmax_t hi, int *rstatus);
long long strsuftoll(const char *desc, const char *val, long long min, long long max);
long long strsuftollx(const char *desc, const char *val, long long min, long long max, char *ebuf, size_t ebuflen);

#ifdef __GLIBC__
#else
uint32_t arc4random(void);
#endif

#ifndef HN_AUTOSCALE
#define HN_AUTOSCALE            0x20
#endif

#ifndef HW_GETSCALE
#define HN_GETSCALE             0x10
#endif

#ifndef HN_DIVISOR_1000
#define HN_DIVISOR_1000         0x08
#endif

#ifndef HN_B
#define HN_B                    0x04
#endif

#ifndef HN_NOSPACE
#define HN_NOSPACE              0x02
#endif

#ifndef HN_DECIMAL
#define HN_DECIMAL              0x01
#endif

#endif
