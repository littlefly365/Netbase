SUBDIRS = cat chmod date domainname echo hostname ln pwd rm rmdir sleep \
	sync date mkdir mv test ksh ls cp expr dd 

all: $(SUBDIRS)

$(SUBDIRS): 
	mkdir -p ../build/bin
	$(MAKE)	-C $@ build
build: $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -rf ../build/bin
.PHONY: all $(SUBDIRS) build
	
