AC_DEFUN([FIND_APR], [
	AC_MSG_CHECKING(for APR)

	AC_ARG_WITH([apr],
		AS_HELP_STRING([--with-apr=PATH], [Find APR in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/bin/apr-1-config ]; then
					AC_MSG_RESULT(yes)
					APR_CONFIG="$withval/bin/apr-1-config"
				elif [ test -f /usr/local/bin/apr-1-config ]; then
					AC_MSG_RESULT(yes)
					APR_CONFIG="/usr/local/bin/apr-1-config"
				elif [ test -f /usr/bin/apr-1-config ]; then
					AC_MSG_RESULT(yes)
					# 'libapr1-dev' package
					APR_CONFIG="/usr/bin/apr-1-config"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find APR. stop)
				fi
				;;
			esac
		],[
			if [ test -f /usr/local/bin/apr-1-config ]; then
				AC_MSG_RESULT(yes)
				APR_CONFIG="/usr/local/bin/apr-1-config"
			elif [ test -f /usr/bin/apr-1-config ]; then
				AC_MSG_RESULT(yes)
				# 'libapr1-dev' package
				APR_CONFIG="/usr/bin/apr-1-config"
			else
				AC_MSG_RESULT(no)
				AC_MSG_ERROR(could not find APR. stop)
			fi
		])
])
