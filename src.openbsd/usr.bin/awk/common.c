/*      $OpenBSD: random.c,v 1.31 2017/11/28 06:55:49 tb Exp $ */
/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
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

#include <stdint.h>
#include <stdlib.h>
#include "nb_stdlib.h"
#include "common.h"

static int random_deterministic;

#define TYPE_0          0               /* linear congruential */
#define BREAK_0         8
#define DEG_0           0
#define SEP_0           0

#define TYPE_3          3               /* x**31 + x**3 + 1 */
#define BREAK_3         128
#define DEG_3           31
#define SEP_3           3

static int32_t randtbl[DEG_3 + 1] = {
        TYPE_3,
        0x991539b1, 0x16a5bce3, 0x6774a4cd, 0x3e01511e, 0x4e508aaa, 0x61048c05,
        0xf5500617, 0x846b7115, 0x6a19892c, 0x896a97af, 0xdb48f936, 0x14898454,
        0x37ffd106, 0xb58bff9c, 0x59e17104, 0xcf918a49, 0x09378c83, 0x52c7a471,
        0x8d293ea9, 0x1f4fc301, 0xc3db71be, 0x39b44e1c, 0xf8a44ef9, 0x4c8b80b1,
        0x19edc328, 0x87bf4bdd, 0xc9b240e5, 0xe9ee4b1b, 0x4382aee7, 0x535b6b41,
        0xf3bec5da,
};

static int32_t *fptr = &randtbl[SEP_3 + 1];
static int32_t *rptr = &randtbl[1];

static int32_t *state = &randtbl[1];
static int32_t *end_ptr = &randtbl[DEG_3 + 1];
static int rand_type = TYPE_3;
static int rand_deg = DEG_3;
static int rand_sep = SEP_3;

static long
random_l(void)
{
        int32_t i;

        if (random_deterministic == 0)
                return arc4random() & 0x7fffffff;

        if (rand_type == TYPE_0)
                i = state[0] = (state[0] * 1103515245 + 12345) & 0x7fffffff;
        else {
                *fptr += *rptr;
                i = (*fptr >> 1) & 0x7fffffff;  /* chucking least random bit */
                if (++fptr >= end_ptr) {
                        fptr = state;
                        ++rptr;
                } else if (++rptr >= end_ptr)
                        rptr = state;
        }
        return((long)i);
}

static void
srandom_l(unsigned int x)
{
        int i;
        int32_t test;
        div_t val;

        random_deterministic = 1;
        if (rand_type == TYPE_0)
                state[0] = x;
        else {
                /* A seed of 0 would result in state[] always being zero. */
                state[0] = x ? x : 1;
                for (i = 1; i < rand_deg; i++) {
                        /*
                         * Implement the following, without overflowing 31 bits:
                         *
                         *      state[i] = (16807 * state[i - 1]) % 2147483647;
                         *
                         *      2^31-1 (prime) = 2147483647 = 127773*16807+2836
                         */
                        val = div(state[i-1], 127773);
                        test = 16807 * val.rem - 2836 * val.quot;
                        state[i] = test + (test < 0 ? 2147483647 : 0);
                }
                fptr = &state[rand_sep];
                rptr = &state[0];
                for (i = 0; i < 10 * rand_deg; i++)
                        (void)random_l();
        }
}

void
srandom_deterministic(unsigned int x)
{
        LOCK();
        srandom_l(x);
        UNLOCK();
}
