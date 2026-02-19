#!/bin/sh
	if [ -f /lib/ld-musl-$(uname -m).so.1 ]; then
		cp rm.c rm.c.bak
		patch -Np0 < ../../patches/linux/0003-unused-rm-musl.patch
	fi
	if [ -f ./rm.c.rej ]; then
		rm -f ./rm.c.rej
	fi
