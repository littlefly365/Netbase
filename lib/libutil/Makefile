SRC= ../logwtmp.c ../y.tab.c ../efun.c

OBJ=obj/*.o
DOBJ=../libnetbsd/obj
EOBJ=$(DOBJ)/strtoi.o $(DOBJ)/strtou.o $(DOBJ)/reallocarr.o
LIB=util
all: pre obj lib
CFLAGS += -Wno-macro-redefined
CPPFLAGS += -include private.h -include sys/nb_cdefs_aout.h -D_EXT_SOURCE

obj: 
	mkdir -p obj && \
	cd obj && \
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRC)
build: obj
	$(AR) rcs lib$(LIB).a $(OBJ) $(EOBJ)
clean:
	rm -rf lib$(LIB).a obj/
.PHONY: all obj build clean
