# Nbase-10
Nbase is a port of the NetBSD's utilities to another unix like operating systems(only tested on Linux with glibc). The number 10 in the name refers to the netbsd version, these  utilities come from the netbsd-10 source.

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
- mv ( you need to apply the patch found in patches/bin and add it to the bin makefile )
- pwd
- rm
- rmdir
- sleep
- sync

# How works?
in the compat/include folder there are some headers for compatibility.
they add some macros like __COPYRIGHT and __RCSID, besides in compat/libcompat are to libraries, libnetbsd(libcompat) and libutil, with these the programs link statically against these librarues but dynamically with the rest of the system ,so no additional runtime depenencies are need(Note: This may change in the future).

# Info
- The compat libraries use only code from the netbsd source tree.
- The goal of this project is port netbsd userland to another systems without any (or tiny) modifications to the original source.

# Questions
- Why i need to patch rm?
A: because the patch is required only for linux. why? A: linux doesnt have st_flags and it is so hard make a wrapper without modify the libc.

# important
if you see build or runtime errors tell me.(Note: The utils are tested on Arch and only with a basic use of them).
