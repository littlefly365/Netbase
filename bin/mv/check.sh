#!/bin/sh
	cp mv.c mv.c.bak
	case $(uname -s) in
		'Linux')
		patch -Np0 < ../../patches/bin/0001-no-fchflags-mv-linux.patch 
		;;
		esac
