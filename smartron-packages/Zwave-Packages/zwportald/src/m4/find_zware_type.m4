AC_DEFUN([FIND_ZWARE_TYPE], [
	AC_MSG_CHECKING(for Z-Ware type)

	AC_ARG_WITH([zware-type],
		AS_HELP_STRING([--with-zware-type=local|portal], [Select Z-Ware type - local or portal]),
		[ case "$withval" in
			local)
				zware_type=local
				AC_MSG_RESULT([${zware_type}])
				;;
			portal)
				zware_type=portal
				AC_MSG_RESULT([${zware_type}])
				;;
			*)
				AC_MSG_ERROR([bad value ${withval} for --with-zware-type])
				;;
			esac
		],[
			zware_type=portal
			AC_MSG_RESULT([${zware_type}])
		])

	AM_CONDITIONAL([ZWARE_LOCAL], [test x$zware_type = xlocal])
	AM_CONDITIONAL([ZWARE_PORTAL], [test x$zware_type = xportal])
])
