AC_DEFUN([FIND_LDAP], [
	AC_MSG_CHECKING(for LDAP)

	AC_ARG_WITH([ldap],
		AS_HELP_STRING([--with-ldap=PATH], [Find ldap in the given PATH]),
		[ case "$withval" in
			no)
				AC_MSG_RESULT(no)
				;;
			*)
				if [ test -d $withval ] && [ test -f $withval/include/ldap.h ] && [ test -f $withval/lib/libldap.so]; then
					AC_MSG_RESULT(yes)
					LDAP_INC="$withval/include"
					LDAP_LIB="$withval/lib"
				elif [ test -f /usr/local/include/ldap.h ] && [ test -f /usr/local/lib/libldap.so]; then
					AC_MSG_RESULT(yes)
					LDAP_INC="/usr/local/include"
					LDAP_LIB="/usr/local/lib"
				elif [ test -f /usr/include/ldap.h ] && [ test -f /usr/lib/i386-linux-gnu/libldap.so]; then
					AC_MSG_RESULT(yes)
					# 'libldap2-dev' package
					LDAP_INC="/usr/include"
					LDAP_LIB="/usr/lib/i386-linux-gnu"
				elif [ test -f /usr/include/ldap.h ] && [ test -f /usr/lib/x86_64-linux-gnu/libldap.so]; then
					AC_MSG_RESULT(yes)
					# 'libldap2-dev' package
					LDAP_INC="/usr/include"
					LDAP_LIB="/usr/lib/x86_64-linux-gnu"
				else
					AC_MSG_RESULT(no)
					AC_MSG_ERROR(could not find ldap. stop)
				fi
				;;
			esac
		],[
			
			AC_MSG_RESULT(no)
			AC_MSG_WARN(not using LDAP)
		])
])
