PROG=users
SRCS=users.c ../who/utmpentry.c

CPPFLAGS+=-I${NETBSDSRCDIR}/usr.bin/who -DSUPPORT_UTMPX -DSUPPORT_UTMP

.include "../../mk/prog.mk"
