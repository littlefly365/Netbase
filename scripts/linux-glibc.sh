#!/bin/sh
	cp $(pwd)/scripts/linux/extattr.h $(pwd)/include/sys/
	cp $(pwd)/scripts/linux/event.h $(pwd)/include/sys/
	if [ ! -f $(pwd)/include/sys/extattr.h ]; then
	echo "error"
	exit 1
	fi
