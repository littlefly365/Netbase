# NetBase
Netbase is a port of the NetBSD's utilities to another unix like operating systems.

# Deps
- bsd make
- gcc or clang
- zlib
- libbz2
- libacl
- libfts ( for musl )
- libmd
- posix shell (link to /bin/sh )

# Info
- The compat libraries use only code from the netbsd source tree(or libbsd).
- The goal of this project is port netbsd userland to another systems with tiny modifications to the original source.
- now are avaible ~80 commands ported from netbsd

# important
if you see build or runtime errors tell me (Note: The utils are tested on Arch/Alpine and only with a basic use of them).

# Compatibility Matrix

| Platform        | Status           |
|-----------------|------------------|
| Linux glibc     | ☑️ Works         |
| Linux musl      | ☑️ Works         |

# Building

# Only Musl
`ln -svf ../netbsd/sys/cdefs.h include/sys/cdefs.h`
`export LBFTS="-lfts"` 

`bmake`

# TODO

|    Plataform/Goal	|     Status	          	|
|-----------------------|-------------------------------|
| glibc		  	| ☑️ yes	                |
| musl		  	| 🧪 WIP	            	|
| another libc		| 🧪 untested               	|
| Full Userland (! GNU) | 🧪 partial (coreutils)	|

