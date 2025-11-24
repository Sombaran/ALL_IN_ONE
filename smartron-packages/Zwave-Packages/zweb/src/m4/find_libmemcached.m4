AC_DEFUN([FIND_LIBMEMCACHED], [
	AC_MSG_CHECKING(for libmemcached)

	AC_ARG_WITH([libmemcached],
		AS_HELP_STRING([--with-libmemcached=PATH], [Find libmemcached in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/libmemcached/memcached.h ] \
					&& [ test -f $withval/lib/libmemcached.a ]; then
					AC_MSG_RESULT(yes)
					LIBMEMCACHED_PATH="$withval"
					LIBMEMCACHED_CFLAGS="-I$withval/include -DZWS_WITH_LIBMEMCACHED"
					LIBMEMCACHED_LIBS="$withval/lib/libmemcached.a -lm -lstdc++"
				elif [ test -f /usr/local/include/libmemcached/memcached.h ] \
					&& [ test -f /usr/local/lib/libmemcached.a ]; then
					AC_MSG_RESULT(yes)
					LIBMEMCACHED_PATH="/usr/local/"
					LIBMEMCACHED_CFLAGS="-I/ust/local/include -DZWS_WITH_LIBMEMCACHED"
					LIBMEMCACHED_LIBS="$/usr/local/lib/libmemcached.a -lm -lstdc++"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find libmemcached. stop)
				fi
				;;
			esac
		],[
			AC_MSG_RESULT(no)
			AC_MSG_WARN(Not using libmemcached)
		])
])
