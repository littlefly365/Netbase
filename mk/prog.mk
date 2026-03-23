.include "config.mk"

SRCS ?= ${PROG}.c
OBJS ?= ${SRCS:.c=.o}

all: ${PROG}

${PROG}: ${OBJS}
	@echo "---${PROG}---"
	@echo "${CC} ${OBJS} -o ${PROG}"
	@$(CC) ${LDFLAGS} ${OBJS} ${LDLIBS} -o ${PROG}
.c.o:
	${CC} ${CPPFLAGS} ${CFLAGS} -c $< -o ./$@
clean:
	rm -rf ${PROG} ${OBJS}
install:
	install -D -m 755 ${PROG} ${DESTDIR}/${BINT}/${PROG}

.if defined(SYMLINKS)
.for s in ${SYMLINKS}
	ln -sf ${PROG} ${DESTDIR}/${BINT}/${s}
.endfor
.endif

.PHONY: all ${PROG} clean install
