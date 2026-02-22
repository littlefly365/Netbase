SUBDIRS = env dirname basename id tee yes head printf wc false true whoami \
	tty uniq nice grep sed nl seq base64 comm expand logname printenv \
	timeout	sort join paste

all: $(SUBDIRS)

$(SUBDIRS): 
	mkdir -p ../build/usr.bin
	$(MAKE)	-C $@ build
build: $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -rf ../build/usr.bin
.PHONY: all $(SUBDIRS) build
	
