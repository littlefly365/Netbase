all: lib bin sbin usr.bin usr.sbin

lib:
	@$(MAKE) -C lib
bin: lib
	@$(MAKE) -C bin
sbin: lib
	@$(MAKE) -C sbin
usr.bin: lib
	@$(MAKE) -C usr.bin
usr.sbin: lib
	@$(MAKE) -C usr.sbin
clean:
	@$(MAKE) -C lib      clean
	@$(MAKE) -C bin      clean
	@$(MAKE) -C sbin     clean
	@$(MAKE) -C usr.bin  clean
	@$(MAKE) -C usr.sbin clean
install:
	@$(MAKE) -C lib      install
	@$(MAKE) -C bin      install
	@$(MAKE) -C sbin     install
	@$(MAKE) -C usr.bin  install
	@$(MAKE) -C usr.sbin install
.PHONY: all lib bin sbin usr.bin usr.sbin clean install
