AC_DEFUN([FIND_SIMCLIST], [
	AC_MSG_CHECKING(for SimCList)

	AC_ARG_WITH([simclist],
		AS_HELP_STRING([--with-simclist=PATH], [Find SimCList library in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/simclist.h ] && [ test -f $withval/lib/libsimclist.so]; then
					AC_MSG_RESULT(yes)
					SIMCLIST_INC="$withval/include"
					SIMCLIST_LIB="$withval/lib"
				elif   [ test -f /usr/local/include/simclist.h ] && [ test -f /usr/local/lib/libsimclist.so]; then
					AC_MSG_RESULT(yes)
					SIMCLIST_INC="/usr/local/include"
					SIMCLIST_LIB="/usr/local/lib"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find SimCList. stop)
				fi
				;;
			esac
		],[
			AC_MSG_RESULT(no)
			AC_MSG_WARN(Not using SimCList)
		])
])
