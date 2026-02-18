# NetBase
Nbase is a port of the NetBSD's utilities to another unix like operating systems(only tested on Linux with glibc).

# Needed
- gnu make (make on linux, gmake on *BSD and macOS)
- clang and llvm
- byacc (berkeley yacc, bison won't work)

# How does it work?
in the compat/include folder there are some headers for compatibility.
they add some macros like __COPYRIGHT and __RCSID, besides in compat/libcompat are two libraries, libnetbsd(libcompat) and libutil, with these the programs link statically against these librarues but dynamically with the rest of the system ,so no additional runtime dependencies are need(Note: This may change in the future).

# Info
- The compat libraries use only code from the netbsd source tree.
- The goal of this project is port netbsd userland to another systems without any (or tiny) modifications to the original source.

# important
if you see build or runtime errors tell me.(Note: The utils are tested on Arch and only with a basic use of them).

# What commands are available?
the commands available are listed on details/

# Compatibility Matrix

| Platform        | Status           |
|-----------------|------------------|
| Linux glibc     | ☑️ Works         |
| Linux musl      | ⚠️ Should work   |
| NetBSD          | ☑️ Works         |
| FreeBSD         | ⚠️ Should work   |
| OpenBSD         | ⚠️ Should work   |
| macOS           | ⚠️ Should work   |
| GNU/Hurd        | ❌ Not supported |
