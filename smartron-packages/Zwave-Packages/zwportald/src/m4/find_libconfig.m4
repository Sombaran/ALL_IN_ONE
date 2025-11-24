AC_DEFUN([FIND_LIBCONFIG], [
	AC_MSG_CHECKING(for libconfig)

	AC_ARG_WITH([libconfig],
		AS_HELP_STRING([--with-libconfig=PATH], [Find libconfig library in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/libconfig.h ] && [ test -f $withval/lib/libconfig.a]; then
					AC_MSG_RESULT(yes)
					LIBCONFIG_INC="$withval/include"
					LIBCONFIG_LIB="$withval/lib"
				elif [ test -f /usr/local/include/libconfig.h ] && [ test -f /usr/local/lib/libconfig.a]; then
					AC_MSG_RESULT(yes)
					LIBCONFIG_INC="/usr/local/include"
					LIBCONFIG_LIB="/usr/local/lib"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find libconfig. stop)
				fi
				;;
			esac
		],[
			if [ test -f /usr/local/include/libconfig.h ] && [ test -f /usr/local/lib/libconfig.a]; then
				AC_MSG_RESULT(yes)
				LIBCONFIG_INC="/usr/local/include"
				LIBCONFIG_LIB="/usr/local/lib"
			else
				AC_MSG_RESULT(no)
				AC_MSG_ERROR(could not find libconfig. stop)
			fi
		])
])
