/*	$NetBSD: df.c,v 1.101.2.1 2023/12/18 14:17:42 martin Exp $ */

/*
 * Copyright (c) 1980, 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
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
 */

#include "sys/nb_cdefs.h"
#ifndef lint
__COPYRIGHT(
"@(#) Copyright (c) 1980, 1990, 1993, 1994\
 The Regents of the University of California.  All rights reserved.");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)df.c	8.7 (Berkeley) 4/2/94";
#else
__RCSID("$NetBSD: df.c,v 1.101.2.1 2023/12/18 14:17:42 martin Exp $");
#endif
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/mount.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <locale.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <util.h>
#include <mntent.h>

#include "nb_stdlib.h"
#include "sys/nb_stat.h"
#include "compat.h"

struct mntinfo {
    unsigned long int f_frsize;
    __fsfilcnt_t f_favail;
    unsigned long int f_namemax;
    unsigned int f_type;    
    unsigned long int f_fsid;
    char *f_mntfromname;          /* mnt_fsname from getmntent */
    char *f_mntfromlabel;	  /* THIS WONT BE USE IT */
    char *f_mntonname;            /* mnt_dir from getmntent */
    char *f_fstypename;           /* mnt_fsname from getmntent */
    char *f_opts;                 /* mnt_opts from getmntent */
    unsigned long f_bsize;        /* f_bsize from statvfs */
    fsblkcnt_t f_blocks;          /* f_blocks from statvfs */
    fsblkcnt_t f_bfree;           /* f_bfree from statvfs */
    fsblkcnt_t f_bavail;          /* f_bavail from statvfs */
    fsfilcnt_t f_files;           /* f_files from statvfs */
    fsfilcnt_t f_ffree;           /* f_ffree from statvfs */
    unsigned long f_flag;         /* f_flag from statvfs */
    dev_t f_dev;                  /* st_dev from stat */
    unsigned int f_selected;      /* used internally here only */
    uid_t f_owner; 		  /* THIS WONT BE USE IT */
    uint64_t f_syncwrites;	  /* THIS WONT BE USE IT */
    uint64_t f_asyncwrites;	  /* THIS WONT BE USE IT */
    fsblkcnt_t f_bresvd;	  /* THIS WONT BE USE IT */
    fsfilcnt_t f_fresvd;	  /* THIS WONT BE USE IT */
   struct statvfs svfs;
};

static char	*getmntpt(const char *);
static void	 addstat(struct statvfs *, const struct statvfs *);
static void	 prtstat(const struct mntinfo *, int);
static int	 selected(const char *, size_t);
static void	 maketypelist(char *);
static size_t	 regetmntinfo(struct mntinfo **, size_t);
__dead static void usage(void);
static void	 prthumanval(int64_t, int);
static void	 prthuman(const struct statvfs *, int64_t, int64_t);
static int	  getmntinfo(struct mntinfo **);
static int	  checkvfsselected(char *);

static int	 aflag, cflag, fflag, gflag, hflag, iflag, lflag;
static int	 Nflag, nflag, Pflag, Wflag;
static long	 usize;
static char	**typelist;
static size_t	 mntcount;

#define WIDTH_INODE	10
#define WIDTH_BLKSIZE	12
static int blksize_width = WIDTH_BLKSIZE;

static int fudgeunits = 0;
static int	  skipvfs_l, skipvfs_t;
static const char **vfslist_l, **vfslist_t;




int
main(int argc, char *argv[])
{
	struct stat stbuf;
	struct mntinfo *mntbuf, totals;
	int ch, maxwidth, width;
	size_t i;
	char *mntpt;

	setprogname(argv[0]);
	(void)setlocale(LC_ALL, "");

	while ((ch = getopt(argc, argv, "abcfGgHhiklmNnPt:W")) != -1)
		switch (ch) {
		case 'a':
			aflag = 1;
			break;
		case 'b':
			hflag = 0;
			usize = 512;
			break;
		case 'c':
			cflag = 1;
			break;
		case 'f':
			fflag = 1;
			break;
		case 'g':
			hflag = 0;
			usize = 1024 * 1024 * 1024;
			break;
		case 'G':
			gflag = 1;
			break;
		case 'H':
			fudgeunits = HN_DIVISOR_1000;
			/* FALL THROUGH */
		case 'h':
			hflag = 1;
			usize = 0;
			break;
		case 'i':
			iflag = 1;
			break;
		case 'k':
			hflag = 0;
			usize = 1024;
			break;
		case 'l':
			lflag = 1;
			break;
		case 'm':
			hflag = 0;
			usize = 1024 * 1024;
			break;
		case 'N':
			Nflag = 1;
			break;
		case 'n':
			nflag = 1;
			break;
		case 'P':
			Pflag = 1;
			break;
		case 'W':
			Wflag = 1;
			break;
		case 't':
			if (typelist != NULL)
				errx(EXIT_FAILURE,
				    "only one -t option may be specified.");
			maketypelist(optarg);
			break;
		case '?':
		default:
			usage();
		}

	if (fflag && (Pflag || gflag))
		errx(EXIT_FAILURE,
		    "only one of -f -G and -P may be specified");
	if (gflag && (Pflag || iflag))
		errx(EXIT_FAILURE,
		    "only one of -G and -P or -i may be specified");
	if (Pflag && iflag)
		errx(EXIT_FAILURE,
		    "only one of -P and -i may be specified");
	if (fflag)
		Nflag = 1;

#if 0
	/*
	 * The block size cannot be checked until after getbsize() is called.
	 */
	if (Pflag && (hflag || (usize != 1024 && usize != 512)))
		errx(EXIT_FAILURE,
		    "non-standard block size incompatible with -P");
#endif
	argc -= optind;
	argv += optind;

	mntcount = getmntinfo(&mntbuf);
	if (mntcount == 0)
		err(EXIT_FAILURE,
		    "retrieving information on mounted file systems");

	if (*argv == NULL) {
		mntcount = regetmntinfo(&mntbuf, mntcount);
	} else {
		if ((mntbuf = calloc(argc, sizeof(*mntbuf))) == NULL)
			err(EXIT_FAILURE, "can't allocate statvfs array");
		mntcount = 0;
		for (/*EMPTY*/; *argv != NULL; argv++) {
			if (stat(*argv, &stbuf) < 0) {
				if ((mntpt = getmntpt(*argv)) == 0) {
					warn("%s", *argv);
					continue;
				}
			} else if (S_ISBLK(stbuf.st_mode)) {
				if ((mntpt = getmntpt(*argv)) == 0)
					mntpt = *argv;
			} else
				mntpt = *argv;
			/*
			 * Statfs does not take a `wait' flag, so we cannot
			 * implement nflag here.
			 */
			if (!statvfs(mntpt, &mntbuf[mntcount].svfs))
				if (lflag &&
				    (mntbuf[mntcount].f_flag & MNT_LOCAL) == 0)
					warnx("Warning: %s is not a local %s",
					    *argv, "file system");
				else if
				    (!selected(mntbuf[mntcount].f_fstypename,
					sizeof(mntbuf[mntcount].f_fstypename)))
					warnx("Warning: %s mounted as a %s %s",
					    *argv,
					    mntbuf[mntcount].f_fstypename,
					    "file system");
				else
					++mntcount;
			else
				warn("%s", *argv);
		}
	}

	if (cflag) {
		memset(&totals, 0, sizeof(totals));
		totals.f_frsize = DEV_BSIZE;
		strlcpy(totals.f_mntfromname, "total",
			sizeof(totals.f_mntfromname));
	}

	maxwidth = 0;
	for (i = 0; i < mntcount; i++) {
		width = 0;
		if (Wflag && mntbuf[i].f_mntfromlabel[0]) {
			/* +5 is for "NAME=" added later */
			width = (int)strlen(mntbuf[i].f_mntfromlabel) + 5;
		}
		if (width == 0)
			width = (int)strlen(mntbuf[i].f_mntfromname);
		if (width > maxwidth)
			maxwidth = width;
		if (cflag)
			addstat(&totals, &mntbuf[i].svfs);
	}

	if (cflag == 0 || fflag == 0)
		for (i = 0; i < mntcount; i++)
			prtstat(&mntbuf[i], maxwidth);

	mntcount = fflag;
	if (cflag)
		prtstat(&totals, maxwidth);

	return 0;
}

static char *
getmntpt(const char *name)
{
	size_t count, i;
	struct mntinfo *mntbuf;
	
	count = getmntinfo(&mntbuf);
	if (count == 0)
		err(EXIT_FAILURE, "Can't get mount information");
	for (i = 0; i < count; i++) {
		#ifdef __linux__
		mntbuf->f_mntfromname=0;
		mntbuf->f_mntonname=0;
		#endif
		if (!strcmp(mntbuf[i].f_mntfromname, name))
			return mntbuf[i].f_mntonname;
	}
	return 0;
}

static enum { IN_LIST, NOT_IN_LIST } which;

static int
selected(const char *type, size_t len)
{
	char **av;

	/* If no type specified, it's always selected. */
	if (typelist == NULL)
		return 1;
	for (av = typelist; *av != NULL; ++av)
		if (!strncmp(type, *av, len))
			return which == IN_LIST ? 1 : 0;
	return which == IN_LIST ? 0 : 1;
}

static void
maketypelist(char *fslist)
{
	size_t i;
	char *nextcp, **av;

	if ((fslist == NULL) || (fslist[0] == '\0'))
		errx(EXIT_FAILURE, "empty type list");

	/*
	 * XXX
	 * Note: the syntax is "noxxx,yyy" for no xxx's and
	 * no yyy's, not the more intuitive "noyyy,noyyy".
	 */
	if (fslist[0] == 'n' && fslist[1] == 'o') {
		fslist += 2;
		which = NOT_IN_LIST;
	} else
		which = IN_LIST;

	/* Count the number of types. */
	for (i = 1, nextcp = fslist;
	    (nextcp = strchr(nextcp, ',')) != NULL; i++)
		++nextcp;

	/* Build an array of that many types. */
	if ((av = typelist = calloc((i + 1), sizeof(*av))) == NULL)
		err(EXIT_FAILURE, "can't allocate type array");
	av[0] = fslist;
	for (i = 1, nextcp = fslist;
	    (nextcp = strchr(nextcp, ',')) != NULL; i++) {
		*nextcp = '\0';
		av[i] = ++nextcp;
	}
	/* Terminate the array. */
	av[i] = NULL;
}

/*
 * Make a pass over the filesystem info in ``mntbuf'' filtering out
 * filesystem types not in ``fsmask'' and possibly re-stating to get
 * current (not cached) info.  Returns the new count of valid statvfs bufs.
 */
static size_t
regetmntinfo(struct mntinfo **mntbufp, size_t count)
{
	size_t i, j;
	struct mntinfo *mntbuf;

	if (!lflag && typelist == NULL && aflag)
		return nflag ? count : (size_t)getmntinfo(&mntbuf);

	mntbuf = *mntbufp;
	j = 0;
	for (i = 0; i < count; i++) {
		if (!aflag && (mntbuf[i].f_flag & MNT_IGNORE) != 0)
			continue;
		if (lflag && (mntbuf[i].f_flag & MNT_LOCAL) == 0)
			continue;
		if (!selected(mntbuf[i].f_fstypename,
		    sizeof(mntbuf[i].f_fstypename)))
			continue;
		if (nflag)
			mntbuf[j] = mntbuf[i];
		else {
			struct mntinfo layerbuf = mntbuf[i];
			(void)statvfs(mntbuf[i].f_mntonname, &mntbuf[j].svfs);
			/*
			 * If the FS name changed, then new data is for
			 * a different layer and we don't want it.
			 */
			if (strcmp(layerbuf.f_mntfromname,
			    mntbuf[j].f_mntfromname))
				mntbuf[j] = layerbuf;
		}
		j++;
	}
	return j;
}

static void
prthumanval(int64_t bytes, int width)
{
	char buf[6];

	(void)humanize_number(buf, sizeof(buf) - (bytes < 0 ? 0 : 1),
	    bytes, "", HN_AUTOSCALE,
	    HN_B | HN_NOSPACE | HN_DECIMAL | fudgeunits);

	(void)printf("%*s", width, buf);
}

static void
prthuman(const struct statvfs *sfsp, int64_t used, int64_t bavail)
{

	prthumanval((int64_t)(sfsp->f_blocks * sfsp->f_frsize), blksize_width);
	prthumanval((int64_t)(used * sfsp->f_frsize), 1 + blksize_width);
	prthumanval((int64_t)(bavail * sfsp->f_frsize), 1 + blksize_width);
}

/*
 * Convert statvfs returned filesystem size into BLOCKSIZE units.
 * Attempts to avoid overflow for large filesystems.
 */
#define fsbtoblk(num, fsbs, bs)					\
	(((fsbs) != 0 && (uint64_t)(fsbs) < (uint64_t)(bs)) ?	\
	    (int64_t)(num) / (int64_t)((bs) / (fsbs)) :		\
	    (int64_t)(num) * (int64_t)((fsbs) / (bs)))

static void
addstat(struct statvfs *totalfsp, const struct statvfs *sfsp)
{
	struct mntinfo *totalsfp, *sfps;
	uint64_t frsize;

	frsize = sfsp->f_frsize / totalfsp->f_frsize;
	totalfsp->f_blocks += sfsp->f_blocks * frsize;
	totalfsp->f_bfree += sfsp->f_bfree * frsize;
	totalfsp->f_bavail += sfsp->f_bavail * frsize;
	totalsfp->f_bresvd += sfps->f_bresvd * frsize;
	totalfsp->f_files += sfsp->f_files;
	totalfsp->f_ffree += sfsp->f_ffree;
	totalfsp->f_favail += sfsp->f_favail;
	totalsfp->f_fresvd += sfps->f_fresvd;
}

/*
 * Print out status about a filesystem.
 */
static void
prtstat(const struct mntinfo *sfps, int maxwidth)
{
	struct statvfs *sfsp = &sfps->svfs;

	static long blocksize = 1024;
	static int headerlen, timesthrough;
	static const char *header;
	static const char full[] = "100";
	static const char empty[] = "  0";
	uint64_t used, availblks, inodes;
	uint64_t bavail;
	char pb[64];
	char mntfromname[128];

	if (Wflag && sfps->f_mntfromlabel[0]) {
		snprintf(mntfromname, sizeof(mntfromname), "NAME=%s",
		    sfps->f_mntfromlabel);
	} else {
		snprintf(mntfromname, sizeof(mntfromname), "%s", sfps->f_mntfromname);
	}

	if (gflag) {
		/*
		 * From SunOS-5.6:
		 *
		 * /var               (/dev/dsk/c0t0d0s3 ):         8192 block size          1024 frag size
		 *   984242 total blocks     860692 free blocks   859708 available         249984 total files
		 *   248691 free files      8388611 filesys id
		 *      ufs fstype       0x00000004 flag             255 filename length
		 *
		 */
		(void)printf("%10s (%-12s): %7ld block size %12ld frag size\n",
		    sfps->f_mntonname, mntfromname,
		    sfsp->f_bsize,	/* On UFS/FFS systems this is
					 * also called the "optimal
					 * transfer block size" but it
					 * is of course the file
					 * system's block size too.
					 */
		    sfsp->f_frsize);	/* not so surprisingly the
					 * "fundamental file system
					 * block size" is the frag
					 * size.
					 */
		(void)printf("%10" PRIu64 " total blocks %10" PRIu64
		    " free blocks  %10" PRId64 " available\n",
		    (uint64_t)sfsp->f_blocks, (uint64_t)sfsp->f_bfree,
		    (uint64_t)sfsp->f_bavail);
		(void)printf("%10" PRIu64 " total files  %10" PRIu64
		    " free files %12lx filesys id\n",
		    (uint64_t)sfsp->f_ffree, (uint64_t)sfsp->f_files,
		    sfsp->f_fsid);
		(void)printf("%10s fstype  %#15lx flag  %17ld filename "
		    "length\n", sfps->f_fstypename, sfsp->f_flag,
		    sfsp->f_namemax);
		(void)printf("%10lu owner %17" PRIu64 " syncwrites %12" PRId64
		    " asyncwrites\n\n", (unsigned long)sfps->f_owner,
		    sfps->f_syncwrites, sfps->f_asyncwrites);

		/*
		 * a concession by the structured programming police to the
		 * indentation police....
		 */
		return;
	}
	if (maxwidth < 12)
		maxwidth = 12;
	if (++timesthrough == 1) {
		switch (blocksize = usize) {
		case 512:
			header = "512-blocks";
			headerlen = (int)strlen(header);
			break;
		case 1024:
			header = Pflag ? "1024-blocks" : "1K-blocks";
			headerlen = (int)strlen(header);
			break;
		case 1024 * 1024:
			header = "1M-blocks";
			headerlen = (int)strlen(header);
			break;
		case 1024 * 1024 * 1024:
			header = "1G-blocks";
			headerlen = (int)strlen(header);
			break;
		default:
			if (hflag) {
				header = "Size";
				headerlen = (int)strlen(header);
				blksize_width = 6;
			} else
				header = getbsize(&headerlen, &blocksize);
			break;
		}

		if (blocksize >= 1024 * 1024)
			blksize_width -= 3;
		if (blocksize >= 1024 * 1024 * 1024)
			blksize_width -= 3;
		if (blksize_width < headerlen)
			blksize_width = headerlen;

		if (Pflag) {
			/*
			 * either:
			 *  "Filesystem 1024-blocks Used Available Capacity Mounted on\n"
			 * or:
			 *  "Filesystem 512-blocks Used Available Capacity Mounted on\n"
			 */
			if (blocksize != 1024 && blocksize != 512)
				errx(EXIT_FAILURE,
				    "non-standard block size incompatible with -P");
			(void)printf("Filesystem %s Used Available Capacity "
			    "Mounted on\n", header);
		} else if (!Nflag) {
			(void)printf("%-*.*s %*s %*s %*s %%Cap",
			    maxwidth, maxwidth, "Filesystem",
			    blksize_width, header,
			    blksize_width, "Used",
			    blksize_width, "Avail");
			if (iflag) {
				(void)printf(" %*s %*s %%iCap",
				    WIDTH_INODE, "iUsed",
				    WIDTH_INODE, "iAvail");
			}
			(void)printf(" Mounted on\n");
		}
	}
	used = (uint64_t)sfps->f_blocks - (uint64_t)sfps->f_bfree;
	bavail = (uint64_t)sfps->f_bavail;
	availblks = bavail + used;
	if (Pflag) {
		assert(hflag == 0);
		assert(blocksize > 0);
		/*
		 * "%s %d %d %d %s %s\n", <file system name>, <total space>,
		 * <space used>, <space free>, <percentage used>,
		 * <file system root>
		 */
		(void)printf("%s %" PRId64 " %" PRId64 " %" PRId64 " %s%% %s\n",
		    sfps->f_mntfromname,
		    fsbtoblk(sfsp->f_blocks, sfsp->f_frsize, blocksize),
		    fsbtoblk(used, sfsp->f_frsize, blocksize),
		    fsbtoblk(bavail, sfsp->f_frsize, blocksize),
		    availblks == 0 ? full : strspct(pb, sizeof(pb), used,
		    availblks, 0), sfps->f_mntonname);
		/*
		 * another concession by the structured programming police to
		 * the indentation police....
		 *
		 * Note iflag cannot be set when Pflag is set.
		 */
		return;
	}

	if (fflag) {
		if (iflag)
			(void)printf("%jd", sfsp->f_ffree);
		else if (hflag)
			prthumanval(bavail * sfsp->f_frsize, 1);
		else
			(void)printf("%jd", fsbtoblk(bavail,
			    sfsp->f_frsize, blocksize));

		if (mntcount != 1)
			(void)printf(" %s\n", sfps->f_mntonname);
		else
			(void)printf("\n");
		return;
	}

	(void)printf("%-*.*s ", maxwidth, maxwidth, mntfromname);

	if (hflag)
		prthuman(sfsp, used, bavail);
	else
		(void)printf("%*" PRId64 " %*" PRId64 " %*" PRId64,
		    blksize_width,
		    fsbtoblk(sfsp->f_blocks, sfsp->f_frsize, blocksize),
		    blksize_width, fsbtoblk(used, sfsp->f_frsize, blocksize),
		    blksize_width, fsbtoblk(bavail, sfsp->f_frsize, blocksize));
	(void)printf(" %3s%%",
	    availblks == 0 ? full :
	    strspct(pb, sizeof(pb), used, availblks, 0));
	if (iflag) {
		inodes = sfsp->f_files;
		used = inodes - sfsp->f_ffree;
		(void)printf(" %*jd %*jd %4s%%",
		    WIDTH_INODE, (intmax_t)used,
		    WIDTH_INODE, (intmax_t)sfsp->f_ffree,
		    inodes == 0 ? (used == 0 ? empty : full) :
		    strspct(pb, sizeof(pb), used, inodes, 0));
	}
	(void)printf(" %s\n", sfps->f_mntonname);
}

static void
usage(void)
{

	(void)fprintf(stderr,
	    "Usage: %s [-aclnW] [-G|-bkP|-bfgHhikmN] [-t type] [file | "
	    "file_system]...\n",
	    getprogname());
	exit(1);
	/* NOTREACHED */
}

static int
getmntinfo(struct mntinfo **mntbuf)
{
    struct mntinfo *list = NULL;
    struct mntinfo *cur;
    struct mntent *ent;
    struct statvfs sv;
    struct stat st;
    FILE *fp;
    int n = 0;

    fp = setmntent("/proc/self/mounts", "r");
    if (fp == NULL)
        err(1, "setmntent");

    while ((ent = getmntent(fp)) != NULL) {

        if (hasmntopt(ent, MNTTYPE_IGNORE))
            continue;

        if (vfslist_l || vfslist_t) {
            if (checkvfsselected(ent->mnt_type) != 0)
                continue;
        }

        if (statvfs(ent->mnt_dir, &sv) == -1)
            continue;

        if (stat(ent->mnt_dir, &st) == -1)
            continue;

        list = realloc(list, (n + 1) * sizeof(*list));
        if (!list)
            err(1, "realloc");

        cur = &list[n];

        memset(cur, 0, sizeof(*cur));

        cur->f_fstypename = strdup(ent->mnt_type);
        cur->f_mntfromname = strdup(ent->mnt_fsname);
        cur->f_mntonname = strdup(ent->mnt_dir);
        cur->f_opts = strdup(ent->mnt_opts);

        cur->f_blocks = sv.f_blocks;
        cur->f_bfree  = sv.f_bfree;
        cur->f_bavail = sv.f_bavail;
        cur->f_files  = sv.f_files;
        cur->f_ffree  = sv.f_ffree;
        cur->f_bsize  = sv.f_bsize;
        cur->f_frsize = sv.f_frsize;
        cur->f_flag   = sv.f_flag;
        cur->f_namemax = sv.f_namemax;

        /* stat */
        cur->f_dev = st.st_dev;

        cur->f_selected = 1;

        n++;
    }

    endmntent(fp);

    *mntbuf = list;
    return n;
}

static int
checkvfsname(const char *vfsname, const char **vfslist, int skip)
{

	if (vfslist == NULL)
		return (0);
	while (*vfslist != NULL) {
		if (strcmp(vfsname, *vfslist) == 0)
			return (skip);
		++vfslist;
	}
	return (!skip);
}

static int
checkvfsselected(char *fstypename)
{
	int result;

	if (vfslist_t) {
		/* if -t option used then select passed types */
		result = checkvfsname(fstypename, vfslist_t, skipvfs_t);
		if (vfslist_l) {
			/* if -l option then adjust selection */
			if (checkvfsname(fstypename, vfslist_l, skipvfs_l) == skipvfs_t)
				result = skipvfs_t;
		}
	} else {
		/* no -t option then -l decides */
		result = checkvfsname(fstypename, vfslist_l, skipvfs_l);
	}
	return (result);
}
