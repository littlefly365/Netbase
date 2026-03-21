# NetBase
Netbase is a port of the NetBSD utilities to another unix like operating systems.

# Deps
- bsd make
- gcc or clang
- zlib
- libbz2
- libacl
- libfts ( for musl ) ( if your distribution does not have it, you can try this https://github.com/littlefly365/libfts )
- libmd
- libcurses ( only tested on ncurses )

# Info
- The compat libraries use only code from the netbsd source tree(or libbsd).
- The goal of this project is port netbsd userland to another systems with tiny modifications to the original source.
- now are available ~100 commands ported from netbsd

# important
- if you see build or runtime errors tell me (Note: The utils are tested on Arch/Alpine and only with a basic use of them).
- This project try to make posible a GNUless system, so with some utilities like diff and diff3 (netbsd does not have it) we will use code from openbsd or freebsd

# Compatibility Matrix

| Platform        | Status           |
|-----------------|------------------|
| Linux glibc     | ☑️ Works          |
| Linux musl      | ☑️ Works          |
| another libc    | 🧪 untested      |

# Building

# Only Musl
`ln -svf ../netbsd/sys/cdefs.h include/sys/cdefs.h`

`export LBFTS="-lfts"` 
# Both

`bmake`

# Status

|    GNU Alternative	|     BSD Alternative	  |
|-----------------------|-------------------------|
| 	coreutils	|	🧪 Partial	  |	
| 	findutils	|	☑️  Complete	  |
| 	diffutils	|	☑️  Complete	  |
| 	grep		|	☑️  Complete	  |
| 	sed		|	☑️  Complete	  |
| 	gawk		|	☑️  Complete	  |
|	bash		|	☑️  Complete (ksh) |
(NOTE: sh is available with nsh name, dont recommend their use (have serious problem) please use ksh instead)
