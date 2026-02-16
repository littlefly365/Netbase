SRC_STDLIB=stdlib/*.c 
SRC_STRING=string/*.c
SRC_UNISTD=unistd/*.c
SRC_TIME=time/*.c
SRC_SYS_STAT=sys/stat/*.c

OBJ=obj/*.o
LIB=netbsd
#CPPFLAGS +=-include locale.h
all: obj lib

obj:
	mkdir -p obj && \
	cd obj && \
	$(CC) $(CFLAGS) $(CPPFLAGS) -c ../$(SRC_STDLIB) && \
	$(CC) $(CFLAGS) $(CPPFLAGS) -c ../$(SRC_STRING) && \
	$(CC) $(CFLAGS) $(CPPFLAGS) -c ../$(SRC_UNISTD) && \
	$(CC) $(CFLAGS) $(CPPFLAGS) -c ../$(SRC_SYS_STAT)
build: obj
	$(AR) rcs lib$(LIB).a $(OBJ)
clean:
	rm -rf lib$(LIB).a obj/
.PHONY: all obj build clean
