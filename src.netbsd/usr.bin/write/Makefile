PROG=write
SRCS=write.c ../who/utmpentry.c term_chk.c

CPPFLAGS+=-DSUPPORT_UTMPX -DSUPPORT_UTMP -I../who -Wno-visibility

.include "../../mk/prog.mk"
