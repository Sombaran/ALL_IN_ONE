AC_DEFUN([FIND_CJSON], [
	AC_MSG_CHECKING(for cJSON)

	AC_ARG_WITH([cjson],
		AS_HELP_STRING([--with-cjson=PATH], [Find cJSON in the given PATH]),
		[ case "$withval" in
                        no)
                                AC_MSG_RESULT(no)
                                ;;
                        *)
                                if [ test -d $withval ] && [ test -f $withval/include/cJSON.h ] \
					&& [ test -f $withval/lib/libcjson.a ]; then
                                        AC_MSG_RESULT(yes)
                                        CJSON_INC="$withval/include"
										CJSON_LIB="$withval/lib"
                                elif [ test -f /usr/local/include/cJSON.h ] \
					&& [ test -f /usr/local/lib/libcjson.a ]; then
                                        AC_MSG_RESULT(yes)
                                        CJSON_INC="/usr/local/include"
										CJSON_LIB="/usr/local/lib"
                                else
                                        AC_MSG_RESULT(no)
                                        AC_MSG_ERROR(could not find cJSON. stop)
                                fi
                                ;;
                        esac
                ],[
						AC_MSG_RESULT(no)
						AC_MSG_WARN(Not using cJSON)
                ])
])
