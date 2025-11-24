AC_DEFUN([FIND_CURL], [
	AC_MSG_CHECKING(for curl)

	AC_ARG_WITH([curl],
		AS_HELP_STRING([--with-curl=PATH], [Find curl in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/curl/curl.h ] \
						&& [ test -f $withval/lib/libcurl.a ]; then
					AC_MSG_RESULT(yes)
					CURL_INC="$withval/include"
					CURL_LIB="$withval/lib"
				elif [ test -f /usr/local/include/curl/curl.h ] \
						&& [ test -f /usr/local/lib/libcurl.a ]; then
					AC_MSG_RESULT(yes)
					CURL_INC="/usr/local/include"
					CURL_LIB="/usr/local/lib"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find curl. stop)
				fi
				;;
			esac
		],[
			if [ test -f /usr/local/include/curl/curl.h ] \
					&& [ test -f /usr/local/lib/libcurl.a ]; then
				AC_MSG_RESULT(yes)
				CURL_INC="/usr/local/include"
				CURL_LIB="/usr/local/lib"
			else
				AC_MSG_RESULT(no)
				AC_MSG_WARN(Not using curl)
			fi
		])
])
