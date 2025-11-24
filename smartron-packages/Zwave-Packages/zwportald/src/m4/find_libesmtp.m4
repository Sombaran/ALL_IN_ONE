AC_DEFUN([FIND_LIBESMTP], [
	AC_MSG_CHECKING(for libESMTP)

	AC_ARG_WITH([libesmtp],
		AS_HELP_STRING([--with-libesmtp=PATH], [Find libESMTP library in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/bin/libesmtp-config ]; then
					AC_MSG_RESULT(yes)
					LIBESMTP_CFLAGS=`$withval/bin/libesmtp-config --cflags`
					LIBESMTP_LIBS=`$withval/bin/libesmtp-config --libs`
				elif   [ test -f /usr/local/include/libesmtp.h ] && [ test -f /usr/local/lib/libesmtp.so]; then
					AC_MSG_RESULT(yes)
					LIBESMTP_CFLAGS="/usr/local/include"
					LIBESMTP_LIBS="/usr/local/lib"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find libESMTP. stop)
				fi
				;;
			esac
		],[
			AC_MSG_RESULT(no)
			AC_MSG_WARN(Not using libESMTP)
		])
])
