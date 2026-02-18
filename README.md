# NetBase
Nbase is a port of the NetBSD's utilities to another unix like operating systems(only tested on Linux with glibc).

# Needed
- gnu make (make on linux, gmake on *BSD and macOS)
- clang and llvm
- byacc (berkeley yacc, bison won't work)

# How works?
in the compat/include folder there are some headers for compatibility.
they add some macros like __COPYRIGHT and __RCSID, besides in compat/libcompat are to libraries, libnetbsd(libcompat) and libutil, with these the programs link statically against these librarues but dynamically with the rest of the system ,so no additional runtime dependencies are need(Note: This may change in the future).

# Info
- The compat libraries use only code from the netbsd source tree.
- The goal of this project is port netbsd userland to another systems without any (or tiny) modifications to the original source.

# Questions
- Why i need to patch rm?
A: because the patch is only required for linux. why? A: linux doesnt have st_flags and it is so hard make a wrapper without modify the libc.

# important
if you see build or runtime errors tell me.(Note: The utils are tested on Arch and only with a basic use of them).

# What's commands are available?
the commands available are listed on details/

# Plataforms supported (❌ not supported, ☑️  works, ⚠️ should work, 🧪 testing)

- GNU/Linux [☑️]
- *Linux [⚠️]
- FreeBSD [⚠️]
- OpenBSD [⚠️]
- NetBSD [☑️]
- MacOS [⚠️]
- Hurd [❌]