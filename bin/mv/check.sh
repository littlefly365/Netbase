#!/bin/sh
	case $(uname -s) in
		'Linux')
		patch -Np0 < ../../patches/linux/0001-no-fchflags-mv-linux.patch 

		if [ -f /lib/ld-musl-$(uname -m).so.1 ]; then
		patch -Np0 < ../../patches/linux/0004-unused-mv-musl.patch 	
		fi
		
		if [ -f mv.c.rej ]; then
		rm mv.c.rej
		fi
		;;
		esac
