AC_DEFUN([FIND_SQLITE], [
	AC_MSG_CHECKING(for sqlite)

	AC_ARG_WITH([sqlite],
		AS_HELP_STRING([--with-sqlite=PATH], [Find sqlite library in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/sqlite3.h ] && [ test -f $withval/lib/libsqlite3.a ]; then
					AC_MSG_RESULT(yes)
					SQLITE_INC="$withval/include"
					SQLITE_LIB="$withval/lib"
				elif [ test -f /usr/local/include/sqlite3.h ] && [ test -f /usr/local/lib/libsqlite3.a ]; then
					AC_MSG_RESULT(yes)
					SQLITE_INC="/usr/local/include"
					SQLITE_LIB="/usr/local/lib"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find sqlite. stop)
				fi
				;;
			esac
		],[
				AC_MSG_RESULT(no)
				AC_MSG_WARN(Not using sqlite)
		])
])
