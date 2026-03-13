.include "config.mk"

OBJS ?= $(SRCS:.c=.o)
CPPFLAGS += -fPIC

all: $(LIB) $(OBJS)

$(LIB): 
	@echo "---lib$(LIB)---"
	@echo "$(CC) $(OBJS) -o lib$(LIB).so"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $(SRCS)
	$(CC) -shared $(OBJS) -o lib$(LIB).so
clean:
	rm -rf lib$(LIB).so $(OBJS)
install:
	 install -D -m 755 lib$(LIB).so $(DESTDIR)/$(LIBDIR)/lib$(LIB).so
.PHONY: all $(LIB)

