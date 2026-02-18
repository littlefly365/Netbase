#!/bin/sh
	cp mv.c mv.c.bak
	case $(uname -s) in
		'Linux')
		if [ -f mv.c.bak ]; then
		SHAOR=$(sha256sum mv.c)
		SHABAK=$(sha256sum mv.c.bak)
		fi
		if [ ! "$SHAOR" = "$SHABAK" ]; then
		patch -Np0 < ../../patches/bin/0001-no-fchflags-mv-linux.patch 
		if [ -f mv.c.rej ]; then
		rm mv.c.rej
			fi
		fi
		;;
		esac
