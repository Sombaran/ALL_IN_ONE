AC_DEFUN([FIND_AVRO], [
	AC_MSG_CHECKING(for Apache Avro)

	AC_ARG_WITH([avro],
		AS_HELP_STRING([--with-avro=PATH], [Find Apache Avro in the given PATH]),
		[ case "$withval" in
                        no)
                                AC_MSG_RESULT(no)
                                ;;
                        *)
                                if [ test -d $withval ] && [ test -f $withval/include/avro.h ] \
					&& [ test -f $withval/lib/libavro.a ]; then
                                        AC_MSG_RESULT(yes)
                                        AVRO_PATH="$withval"
                                elif [ test -f /usr/local/include/avro.h ] \
					&& [ test -f /usr/local/lib/libavro.a ]; then
                                        AC_MSG_RESULT(yes)
                                        AVRO_PATH="/usr/local/"
                                else
                                        AC_MSG_RESULT(no)
                                        AC_MSG_ERROR(could not find Apache Avro. stop)
                                fi
                                ;;
                        esac
                ],[
			if [ test -f /usr/local/include/avro.h ] \
				&& [ test -f /usr/local/lib/libavro.a ]; then
                                AC_MSG_RESULT(yes)
				AVRO_PATH="/usr/local/"
                        else
                                AC_MSG_RESULT(no)
				AC_MSG_ERROR(could not find Apache Avro. stop)
                        fi
                ])
])
