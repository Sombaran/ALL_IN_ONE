AC_DEFUN([FIND_HCAPI_SECURITY], [
	AC_MSG_CHECKING(for hcapi security)

	AC_ARG_WITH([hcapi-security],
		AS_HELP_STRING([--with-hcapi-security=openssl|axtls], [Select OpenSSL or axTLS]),
		[ case "$withval" in
			no)
				HCAPI_SECURITY=openssl
				AC_MSG_RESULT([${HCAPI_SECURITY}])
				;;
			*)
				ol_arg=invalid
				for ol_val in openssl axtls ; do
					if test "$withval" = "$ol_val" ; then
						ol_arg="$ol_val"
					fi
				done

				if test "$ol_arg" = "invalid" ; then
					 AC_MSG_ERROR([Invalid security option '$withval' for HCAPI])
				fi

				HCAPI_SECURITY=$ol_arg
				AC_MSG_RESULT([${HCAPI_SECURITY}])
				;;
			esac

		],[
			HCAPI_SECURITY=openssl
			AC_MSG_RESULT([${HCAPI_SECURITY}])
		])

])

AC_DEFUN([FIND_HCAPI], [
	AC_MSG_CHECKING(for hcapi)

	AC_ARG_WITH([hcapi],
		AS_HELP_STRING([--with-hcapi=PATH], [Find HCAPI in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/zip_api.h ] \
						&& [ test -f $withval/lib/libzip_api.a ] \
						&& [ test -f $withval/lib/libzip_ctl.a ] \
						&& ([ test "${HCAPI_SECURITY}" != "axtls" ] \
							|| [ test -f $withval/lib/libaxtls.a ]); then
					AC_MSG_RESULT(yes)
					HCAPI_INC="$withval/include"
					HCAPI_LIB_ZWAPI="$withval/lib/libzip_api.a"
					HCAPI_LIB_HCAPI="$withval/lib/libzip_ctl.a"
					if [ test "${HCAPI_SECURITY}" = "axtls" ]; then
						HCAPI_SECURITY_INC_FLAGS="-I$withval/include"
						HCAPI_SECURITY_LDFLAGS="-L$withval/lib -Wl,-rpath -Wl,$withval/lib"
						HCAPI_SECURITY_LIBS="-laxtls"
					elif [ test "${HCAPI_SECURITY}" = "openssl" ]; then
						HCAPI_SECURITY_INC_FLAGS="-I${OPENSSL_INC} -I${OPENSSL_INC}/openssl"
						HCAPI_SECURITY_LDFLAGS="-L${OPENSSL_LIB} -Wl,-rpath -Wl,${OPENSSL_LIB}"
						HCAPI_SECURITY_LIBS="-lssl -lcrypto"
					fi
				elif [ test -d $withval ] && [ test -f /usr/local/include/zip_api.h ] \
						&& [ test -f /usr/local/lib/libzip_api.a] \
						&& [ test -f /usr/local/lib/libzip_ctl.a] \
						&& ([ test "${HCAPI_SECURITY}" != "axtls" ] \
							|| [ test -f /usr/local/lib/libaxtls.a ]); then
					AC_MSG_RESULT(yes)
					HCAPI_INC="/usr/local/include"
					HCAPI_LIB_ZWAPI="/usr/local/lib/libzip_api.a"
					HCAPI_LIB_HCAPI="/usr/local/lib/libzip_ctl.a"
					if [ test "${HCAPI_SECURITY}" = "axtls" ]; then
						HCAPI_SECURITY_INC_FLAGS="-I/usr/local/include"
						HCAPI_SECURITY_LDFLAGS="-L/usr/local/lib/ -Wl,-rpath -Wl,/usr/local/lib/"
						HCAPI_SECURITY_LIBS="-laxtls"
					elif [ test "${HCAPI_SECURITY}" = "openssl" ]; then
						HCAPI_SECURITY_INC_FLAGS="-I${OPENSSL_INC} -I${OPENSSL_INC}/openssl"
						HCAPI_SECURITY_LDFLAGS="-L${OPENSSL_LIB} -Wl,-rpath -Wl,${OPENSSL_LIB}"
						HCAPI_SECURITY_LIBS="-lssl -lcrypto"
					fi
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find hcapi. stop)
				fi
				;;
			esac
		],[
			if [ test -d $withval ] && [ test -f /usr/local/include/zip_api.h ] \
					&& [ test -f /usr/local/lib/libzip_api.a] \
					&& [ test -f /usr/local/lib/libzip_ctl.a] \
					&& ([ test "${HCAPI_SECURITY}" != "axtls" ] \
						|| [ test -f /usr/local/lib/libaxtls.a ]); then
				AC_MSG_RESULT(yes)
				HCAPI_INC="/usr/local/include"
				HCAPI_LIB_ZWAPI="/usr/local/lib/libzip_api.a"
				HCAPI_LIB_HCAPI="/usr/local/lib/libzip_ctl.a"
				if [ test "${HCAPI_SECURITY}" = "axtls" ]; then
					HCAPI_SECURITY_INC_FLAGS="-I/usr/local/include"
					HCAPI_SECURITY_LDFLAGS="-L/usr/local/lib/ -Wl,-rpath -Wl,/usr/local/lib/"
					HCAPI_SECURITY_LIBS="-laxtls"
				elif [ test "${HCAPI_SECURITY}" = "openssl" ]; then
					HCAPI_SECURITY_INC_FLAGS="-I${OPENSSL_INC} -I${OPENSSL_INC}/openssl"
					HCAPI_SECURITY_LDFLAGS="-L${OPENSSL_LIB} -Wl,-rpath -Wl,${OPENSSL_LIB}"
					HCAPI_SECURITY_LIBS="-lssl -lcrypto"
				fi
			else
				AC_MSG_RESULT(no)
				AC_MSG_ERROR(could not find hcapi. stop)
			fi
		])
])
