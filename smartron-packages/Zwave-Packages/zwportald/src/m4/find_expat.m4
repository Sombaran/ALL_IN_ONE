AC_DEFUN([FIND_EXPAT], [

	AC_ARG_WITH([expat],
    AS_HELP_STRING([--with-expat=PATH], [Find Expat library in the given PATH]),
    expat_path=${withval},
    expat_path="")
    
	# echo "expat_path is ${expat_path}"    
	if [[ -n "${expat_path}" ]]; then
	  export PKG_CONFIG_PATH="${expat_path}"/lib/pkgconfig/:$PKG_CONFIG_PATH;
	fi
	# echo "PKG_CONFIG_PATH is $PKG_CONFIG_PATH"
	PKG_CHECK_MODULES([EXPAT], [expat >= 2.1.0])
])
