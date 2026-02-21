SRC= ../logwtmp.c ../y.tab.c

OBJ=obj/*.o
LIB=util
all: pre obj lib
CPPFLAGS += -include private.h -include sys/nb_cdefs_aout.h -D_EXT_SOURCE

obj: 
	mkdir -p obj && \
	cd obj && \
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRC)
build: obj
	$(AR) rcs lib$(LIB).a $(OBJ)
clean:
	rm -rf lib$(LIB).a obj/
.PHONY: all obj build clean
