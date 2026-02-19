#!/bin/sh
	cp $(pwd)/scripts/linux/extattr.h $(pwd)/include/sys/
	if [ -f $(pwd)/include/sys/extattr.h ]; then
	make -j"$(nproc)"
	else
	echo "error"
	exit 1
	fi
