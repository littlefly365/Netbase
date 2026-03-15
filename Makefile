all: netbsd openbsd

netbsd:
	@$(MAKE) -C src.netbsd
openbsd: netbsd
	@$(MAKE) -C src.openbsd
clean:
	@$(MAKE) -C src.netbsd      clean
	@$(MAKE) -C src.openbsd     clean
install:
	@$(MAKE) -C src.netbsd      install
	@$(MAKE) -C src.openbsd     install
.PHONY: all netbsd openbsd clean install
