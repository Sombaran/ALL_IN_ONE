AC_DEFUN([FIND_ZLIB], [
	
	AC_ARG_WITH([zlib],
    AS_HELP_STRING([--with-zlib=PATH], [Find Zlib in the given PATH]),
    zlib_path=${withval},
    zlib_path="")
    
	# echo "zlib_path is ${zlib_path}"    
	if [[ -n "${zlib_path}" ]]; then
	  export PKG_CONFIG_PATH="${zlib_path}"/lib/pkgconfig/:$PKG_CONFIG_PATH;
	fi
	# echo "PKG_CONFIG_PATH is $PKG_CONFIG_PATH"
	PKG_CHECK_MODULES([ZLIB], [zlib >= 1.2.8])
])
