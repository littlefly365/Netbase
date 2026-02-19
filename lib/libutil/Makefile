SRC= ../logwtmp.c ../y.tab.c #../strptime.c

OBJ=obj/*.o
LIB=util
all: pre obj lib
CPPFLAGS += -include private.h
pre:
	byacc -d parsedate.y
obj: pre
	mkdir -p obj && \
	cd obj && \
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRC)
build: obj
	$(AR) rcs lib$(LIB).a $(OBJ)
clean:
	rm -rf lib$(LIB).a obj/ y.tab.*
.PHONY: all pre obj build clean
