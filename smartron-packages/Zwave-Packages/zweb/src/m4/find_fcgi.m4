AC_DEFUN([FIND_FCGI], [
	AC_MSG_CHECKING(for FastCGI)

	AC_ARG_WITH([fcgi],
		AS_HELP_STRING([--with-fcgi=PATH], [Find FastCGI in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/fcgi_stdio.h ] \
					&& [ test -f $withval/lib/libfcgi.a ]; then
					AC_MSG_RESULT(yes)
					FCGI_PATH="$withval"
					FCGI_CFLAGS="-I$withval/include -DZWS_WITH_FCGI"
					FCGI_LIBS="$withval/lib/libfcgi.a"
				elif [ test -f /usr/local/include/fcgi_stdio.h ] \
					&& [ test -f /usr/local/lib/libfcgi.a ]; then
					AC_MSG_RESULT(yes)
					FCGI_PATH="/usr/local/"
					FCGI_CFLAGS="-I/ust/local/include -DZWS_WITH_FCGI"
					FCGI_LIBS="$/usr/local/lib/libfcgi.a"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find FastCGI. stop)
				fi
				;;
			esac
		],[
			AC_MSG_RESULT(no)
			AC_MSG_WARN(Not using FastCGI)
		])
])
