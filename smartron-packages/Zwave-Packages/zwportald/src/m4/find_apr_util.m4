AC_DEFUN([FIND_APR_UTIL], [
	AC_MSG_CHECKING(for APR-util)

	AC_ARG_WITH([apr-util],
		AS_HELP_STRING([--with-apr-util=PATH], [Find APR-util in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/bin/apu-1-config ]; then
					AC_MSG_RESULT(yes)
					APR_UTIL_CONFIG="$withval/bin/apu-1-config"
				elif [ test -f /usr/local/bin/apu-1-config ]; then
					AC_MSG_RESULT(yes)
					APR_UTIL_CONFIG="/usr/local/bin/apu-1-config"
				elif [ test -f /usr/bin/apu-1-config ]; then
					AC_MSG_RESULT(yes)
					# 'libaprutil1-dev' package
					APR_UTIL_CONFIG="/usr/bin/apu-1-config"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find APR-util. stop)
				fi
				;;
			esac
		],[
			if [ test -f /usr/local/bin/apu-1-config ]; then
				AC_MSG_RESULT(yes)
				APR_UTIL_CONFIG="/usr/local/bin/apu-1-config"
			elif [ test -f /usr/bin/apu-1-config ]; then
				AC_MSG_RESULT(yes)
				# 'libaprutil1-dev' package
				APR_UTIL_CONFIG="/usr/bin/apu-1-config"
			else
				AC_MSG_RESULT(no)
				AC_MSG_ERROR(could not find APR-util. stop)
			fi
		])
])
