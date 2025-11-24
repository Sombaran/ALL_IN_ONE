AC_DEFUN([FIND_LIBFFI], [
	
	AC_ARG_WITH([libffi],
    AS_HELP_STRING([--with-libffi=PATH], [Find libffi in the given PATH]),
    libffi_path=${withval},
    libffi_path="")
    
	# echo "libffi_path is ${libffi_path}"    
	if [[ -n "${libffi_path}" ]]; then
	  export PKG_CONFIG_PATH="${libffi_path}"/lib/pkgconfig/:$PKG_CONFIG_PATH;
	fi
	# echo "PKG_CONFIG_PATH is $PKG_CONFIG_PATH"
	PKG_CHECK_MODULES([LIBFFI], [libffi >= 3.0.13])
])
