AC_DEFUN([FIND_GLIB], [
	
	AC_ARG_WITH([glib],
    AS_HELP_STRING([--with-glib=PATH], [Find Glib in the given PATH]),
    glib_path=${withval},
    glib_path="")
    
	# echo "glib_path is ${glib_path}"    
	if [[ -n "${glib_path}" ]]; then
	  export PKG_CONFIG_PATH="${glib_path}"/lib/pkgconfig/:$PKG_CONFIG_PATH;
	fi
	# echo "PKG_CONFIG_PATH is $PKG_CONFIG_PATH"
	PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.37.93])
])
