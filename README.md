# NetBase
Netbase is a port of the NetBSD's utilities to another unix like operating systems.

# Deps
- gnu make
- clang and llvm
- byacc (bison will NOT work)
- zlib
- libbz2
- libfts ( for musl )
- posix shell (link to /bin/sh )

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
| Linux glibc     | ☑️ Works          |
| Linux musl      | ☑️ Works          |
| NetBSD          | ☑️ Works          |
| FreeBSD         | 🧪 Should work   |
| OpenBSD         | 🧪 Should work   |
| macOS           | 🧪 Should work   |
| GNU/Hurd        | ⚠️ May work       |

# Building
if you run only g/make may fail so i recommend execute the build script instead:
`./build.sh`       
use:

`-b, --build` 

run make with configs and variables depending on the platform (default).

`-c, --clean`
remove object files and clean the source tree.

# TODO

|    Plataform/Goal	|    Status	|
|-----------------------|---------------|
| linux (glibc/musl)  	| ☑️  yes	|
| Free/Open/NetBSD 	| 🧪should work |
| MacOS/Minix/Hurd	| ⚠️  untested	|
| Full Userland (! GNU) | 		|

