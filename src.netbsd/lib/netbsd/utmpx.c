/*      $NetBSD: utmpx.c,v 1.35 2015/05/23 11:48:13 christos Exp $       */

/*-
 * Copyright (c) 2002 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christos Zoulas.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sys/nb_cdefs.h"
#include "nb_assert.h"
#include "sys/nb_param.h"
#include "nb_utmpx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utmpx.h>

static FILE *fp;
static int readonly = 0;
static char utfile[MAXPATHLEN] = _PATH_UTMPX;
static struct utmpx ut;

void
endutxent(void)
{

        (void)memset(&ut, 0, sizeof(ut));
        if (fp != NULL) {
                (void)fclose(fp);
                fp = NULL;
                readonly = 0;
        }
}

int
utmpxname(const char *fname)
{
        size_t len;

        _DIAGASSERT(fname != NULL);

        len = strlen(fname);

        if (len >= sizeof(utfile))
                return 0;

        /* must end in x! */
        if (fname[len - 1] != 'x')
                return 0;

        (void)strlcpy(utfile, fname, sizeof(utfile));
        endutxent();
        return 1;
}

