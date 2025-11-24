AC_DEFUN([FIND_LIBFCGI], [
	AC_MSG_CHECKING(for libfcgi)

	AC_ARG_WITH([libfcgi],
		AS_HELP_STRING([--with-libfcgi=PATH], [Find libfcgi library in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/fastcgi.h ] && [ test -f $withval/lib/libfcgi.a]; then
					AC_MSG_RESULT(yes)
					LIBFCGI_INC="$withval/include"
					LIBFCGI_LIB="$withval/lib"
				elif [ test -f /usr/local/include/fastcgi.h ] && [ test -f /usr/local/lib/libfcgi.a]; then
					AC_MSG_RESULT(yes)
					LIBFCGI_INC="/usr/local/include"
					LIBFCGI_LIB="/usr/local/lib"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find libfcgi. stop)
				fi
				;;
			esac
		],[
			if [ test -f /usr/local/include/fastcgi.h ] && [ test -f /usr/local/lib/libfcgi.a]; then
				AC_MSG_RESULT(yes)
				LIBFCGI_INC="/usr/local/include"
				LIBFCGI_LIB="/usr/local/lib"
			else
				AC_MSG_RESULT(no)
				AC_MSG_ERROR(could not find libfcgi. stop)
			fi
		])
])
