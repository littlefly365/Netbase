#!/bin/sh
	mkdir -p $(pwd)/include/protocols
	cp $(pwd)/scripts/linux/extattr.h $(pwd)/include/sys/ && \
	cp $(pwd)/scripts/musl/cdefs.h $(pwd)/include/sys/ && \
	cp $(pwd)/scripts/musl/queue.h $(pwd)/include/sys/ && \
	cp $(pwd)/scripts/musl/timed.h $(pwd)/include/protocols/

	if [ -f $(pwd)/include/sys/extattr.h ]; then
		if [ -f $(pwd)/include/sys/cdefs.h ]; then		
			make -j"$(nproc)"
		else
			echo "error"
			exit 1
		fi
	else
		echo "error"
		exit 1
	fi
