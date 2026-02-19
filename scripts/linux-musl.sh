#!/bin/sh
	cp $(pwd)/scripts/extattr.h $(pwd)/include/sys/
	make -j"$(nproc)"
