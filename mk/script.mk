.include "config.mk"
all: copy chmod 

chmod: copy
	chmod +x $(PROG)
copy:
	cp $(PROG).sh $(PROG)
clean:
	@rm -f $(PROG)
install:
	@install -D -m 755 $(PROG) $(DESTDIR)/$(BINT)/$(PROG)

.PHONY: all chmod copy clean install
