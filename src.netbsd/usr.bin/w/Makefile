PROG=w
SRCS=../../bin/ps/fmt.c pr_time.c w.c

LDADD +=-lnutil
SYMLINKS= uptime
CPPFLAGS+= -DSUPPORT_UTMP -DSUPPORT_UTMPX -Wno-format-y2k

.include "../../mk/prog.mk"
