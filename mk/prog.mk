.include "config.mk"

SRCS ?= $(PROG).c
OBJS ?= $(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	@echo "---$(PROG)---"
	@echo "$(CC) $(OBJS) -o $(PROG)"
	@$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(PROG)
.c.o: 
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(PROG) $(OBJS)
install:
	install -D -m 755 $(PROG) $(DESTDIR)/$(BINT)/$(PROG)
.PHONY: all $(PROG) clean install
