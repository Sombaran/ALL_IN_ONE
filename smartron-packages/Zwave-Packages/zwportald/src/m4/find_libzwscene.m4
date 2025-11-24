AC_DEFUN([FIND_LIBZWSCENE], [

	AC_ARG_WITH([libzwscene],
    AS_HELP_STRING([--with-libzwscene=PATH], [Find ZWScene library in the given PATH]),
    libzwscene_path=${withval},
    libzwscene_path="")
    
	# echo "libzwscene_path is ${libzwscene_path}"    
	if [[ -n "${libzwscene_path}" ]]; then
	  export PKG_CONFIG_PATH="${libzwscene_path}"/lib/pkgconfig/:$PKG_CONFIG_PATH;
	fi
	# echo "PKG_CONFIG_PATH is $PKG_CONFIG_PATH"
	PKG_CHECK_MODULES([LIBZWSCENE], [libzwscene >= 4.2.0])
	
])
