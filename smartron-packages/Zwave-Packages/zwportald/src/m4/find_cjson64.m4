AC_DEFUN([FIND_CJSON64], [
	AC_MSG_CHECKING(for cJSON64)

	AC_ARG_WITH([cjson64],
		AS_HELP_STRING([--with-cjson64=PATH], [Find cJSON64 lib in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/cJSON64.h ] \
						&& [ test -f $withval/lib/cJSON64.a ]; then
					AC_MSG_RESULT(yes)
					CJSON64_INC="$withval/include"
					CJSON64_LIB="$withval/lib"
				elif [ test -f /usr/local/include/cJSON64.h ] \
						&& [ test -f /usr/local/lib/cJSON64.a ]; then
					AC_MSG_RESULT(yes)
					CJSON64_INC="/usr/local/include"
					CJSON64_LIB="/usr/local/lib"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find cJSON64. stop)
				fi
				;;
			esac
		],[
			if [ test -f /usr/local/include/cJSON64.h ] \
					&& [ test -f /usr/local/lib/cJSON64.a]; then
				AC_MSG_RESULT(yes)
				CJSON64_INC="/usr/local/include"
				CJSON64_LIB="/usr/local/lib"
			else
				AC_MSG_RESULT(no)
				AC_MSG_WARN(Not using cJSON64)
			fi
		])
])
