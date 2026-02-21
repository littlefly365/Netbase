/*      $NetBSD: stat.h,v 1.69 2019/09/15 23:55:22 christos Exp $       */

/*-
 * Copyright (c) 1982, 1986, 1989, 1993
 *      The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *      @(#)stat.h      8.12 (Berkeley) 8/17/94
 */

#ifndef _SYS_NB_STAT_H
#define _SYS_NB_STAT_H

#ifndef S_ISTXT
#define	S_ISTXT	0001000
#endif

#ifndef _S_IFWHT
#define	_S_IFWHT  0160000
#endif

#ifndef S_IFWHT
#define	S_IFWHT  _S_IFWHT
#endif

#ifndef S_ISWHT
#define	S_ISWHT(m)	(((m) & _S_IFMT) == _S_IFWHT)
#endif

#ifndef _S_IFMT
#define	_S_IFMT	  0170000
#endif

#ifndef S_IFMT
#define	S_IFMT	 _S_IFMT
#endif 

#ifndef DEFFILEMODE
#define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#endif
int fchflags(int fd, unsigned long flags);

#ifndef ALLPERMS
#define     ALLPERMS        (S_ISUID|S_ISGID|S_ISTXT|S_IRWXU|S_IRWXG|S_IRWXO)
#endif

int lutimens(const char *path, const struct timespec *times);

#endif
