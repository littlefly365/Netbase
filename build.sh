#!/bin/sh
	case "$1" in
		''| -b | --build)
			case "$(uname -s)" in
					Linux)
						if ( ldd --version | grep GNU >/dev/null ); then
						scripts/linux-glibc.sh
						else
						scripts/linux-musl.sh
						fi
					;;
					*)
						scripts/bsd.sh
					;;
					esac
			
			;;
		-c | --clean)
			if command -v gmake >/dev/null 2>&1; then
				gmake clean
			else
				make clean
			fi
		;;
		*)
		printf "usage:\n"
		printf "\t-b, --build\tbuild the project\n"
		printf "\t-c, --clean\tclean the source tree\n"
		exit 1
		;;
		esac
