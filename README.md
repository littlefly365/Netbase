# Nbase-10
Nbase is a port of the NetBSD's utilities to another unix like operating systems(only tested on Linux with glibc). The number 10 in the name means the netbsd version, in this case the utilities are from netbsd-10.

# Needed
- gmake
- clang and llvm
- byacc

# Avaible commands
- cat
- chmod
- date
- domianname
- echo
- hostname
- ln
- mv ( you need to apply the patch found in patches/bin and add to the bin makefile )
- pwd
- rm
- rmdir
- sleep
- sync
