AC_DEFUN([FIND_OPENSSL], [
	AC_MSG_CHECKING(for openssl)

	AC_ARG_WITH([openssl],
		AS_HELP_STRING([--with-openssl=PATH], [Find OpenSSL in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/openssl/ssl.h ] \
						&& [ test -f $withval/lib/libssl.a ]; then
					AC_MSG_RESULT(yes)
					OPENSSL_INC="$withval/include"
					OPENSSL_LIB="$withval/lib"
				elif [ test -f /usr/local/include/openssl/ssl.h ] \
						&& [ test -f /usr/local/lib/libssl.a ]; then
					AC_MSG_RESULT(yes)
					OPENSSL_INC="/usr/local/include"
					OPENSSL_LIB="/usr/local/lib"
				elif [ test -f /usr/include/openssl/ssl.h ] \
						&& [ test -f /lib/i386-linux-gnu/libssl.so* ]; then
					AC_MSG_RESULT(yes)
					# Header files from 'libssl-dev' package
					OPENSSL_INC="/usr/include"
					OPENSSL_LIB="/lib/i386-linux-gnu"
				elif [ test -f /usr/include/openssl/ssl.h ] \
						&& [ test -f /lib/x86_64-linux-gnu/libssl.so* ]; then
					AC_MSG_RESULT(yes)
					# Header files from 'libssl-dev' package
					OPENSSL_INC="/usr/include"
					OPENSSL_LIB="/lib/x86_64-linux-gnu"
				else
					AC_MSG_RESULT(no)
					if [ test "${HCAPI_SECURITY}" = "openssl" ]; then
						AC_MSG_ERROR(could not find openssl. stop)
					fi
				fi
				;;
			esac
		],[
			if [ test -f /usr/local/include/openssl/ssl.h ] \
					&& [ test -f /usr/local/lib/libssl.a]; then
				AC_MSG_RESULT(yes)
				OPENSSL_INC="/usr/local/include"
				OPENSSL_LIB="/usr/local/lib"
			elif [ test -f /usr/include/openssl/ssl.h ] \
					&& [ test -f /lib/i386-linux-gnu/libssl.so* ]; then
				AC_MSG_RESULT(yes)
				# Header files from 'libssl-dev' package
				OPENSSL_INC="/usr/include"
				OPENSSL_LIB="/lib/i386-linux-gnu"
			elif [ test -f /usr/include/openssl/ssl.h ] \
					&& [ test -f /lib/x86_64-linux-gnu/libssl.so* ]; then
				AC_MSG_RESULT(yes)
				# Header files from 'libssl-dev' package
				OPENSSL_INC="/usr/include"
				OPENSSL_LIB="/lib/x86_64-linux-gnu"
			else
				AC_MSG_RESULT(no)
				if [ test "${HCAPI_SECURITY}" = "openssl" ]; then
					AC_MSG_WARN(could not find openssl. continuing)
				fi
			fi
		])
])
