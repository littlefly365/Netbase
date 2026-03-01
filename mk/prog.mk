.include "../../mk/config.mk"

SRCS ?= $(PROG).c
OBJS ?= $(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	@echo "---$(PROG)---"
	@echo "$(CC) $(OBJS) -o $(PROG)"
	@$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(PROG)
.c.o: 
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ >/dev/null 2>&1
clean:
	rm -f $(PROG) $(OBJS)

.PHONY: all $(PROG) clean
