#!/bin/sh
	cp trap.c trap.c.bak
	case $(uname -s) in
		'Linux')
		if [ -f trap.c.bak ]; then
		SHAOR=$(sha256sum trap.c)
		SHABAK=$(sha256sum trap.c.bak)
		fi
		if [ ! "$SHAOR" = "$SHABAK" ]; then
		patch -Np0 < ../../patches/bin/0002-strsignal-ksh-linux.patch 
		if [ -f trap.c.rej ]; then
		rm trap.c.rej
			fi
		fi
		;;
		esac
