PROG=nsh
ECHO=bltin/echo.c

SRCS=alias.c arith_token.c arithmetic.c cd.c ${ECHO} error.c eval.c exec.c \
	expand.c histedit.c input.c jobs.c mail.c main.c memalloc.c \
	miscbltin.c mystring.c options.c parser.c redir.c show.c trap.c \
	output.c var.c test.c kill.c syntax.c printf.c builtins.c init.c nodes.c 
LDADD+=	-ledit 

CPPFLAGS+=-DSHELL -I. -I${.CURDIR}/editline
CPPFLAGS+= -DUSE_LRAND48 -Wno-unused-value

.include "../../mk/prog.mk"
