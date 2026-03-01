SRC= ../logwtmp.c ../y.tab.c ../efun.c ../rump.c

OBJ=obj/*.o
DOBJ=../libnetbsd/obj
EOBJ=$(DOBJ)/strtoi.o $(DOBJ)/strtou.o $(DOBJ)/reallocarr.o
LIB=libnutil.so
CFLAGS += -Wno-macro-redefined -fPIC
CPPFLAGS += -include private.h 

all: obj lib

obj: 
	mkdir -p obj && \
	cd obj && \
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRC)
build: obj
	$(CC) -shared $(OBJ) $(EOBJ) -o $(LIB)
clean:
	rm -rf $(LIB) obj/
.PHONY: all obj build clean
