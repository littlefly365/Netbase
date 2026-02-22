#!/bin/sh
cores="$2"
if [ ! -z "$2" ]; then
	cores="$(nproc)"
fi
	case "$1" in
		''| -b | --build)
			case "$(uname -s)" in
					Linux)
						if  ldd --version 2>&1 | grep -q musl; then
							if scripts/linux-musl.sh; then
								make -j"$cores"
							fi
						elif  ldd --version 2>&1 | grep -q GNU; then
							if scripts/linux-glibc.sh; then
								make -j"$cores"
							fi
						else
							echo "error"
						fi
					;;
					*)
						echo "Your plataform is not supported"
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
		printf "first argument:\n"
		printf "\t-b, --build\tbuild the project\n"
		printf "\t-c, --clean\tclean the source tree\n"
		printf "second argument:\n"
		printf "\t-4 ( or core count )\tuse the number of cores for build\n"
		exit 1
		;;
		esac
