AC_DEFUN([FIND_UDUNITS], [
	
	AC_ARG_WITH([udunits],
    AS_HELP_STRING([--with-udunits=PATH], [Find UDUnits in the given PATH]),
    udunits_path=${withval},
    udunits_path="")
    
	# echo "udunits_path is ${udunits_path}"    
	if [[ -n "${udunits_path}" ]]; then
	  UDUNITS_CFLAGS="-I${udunits_path}/include"
	  UDUNITS_LIBS="-L${udunits_path}/lib -ludunits2"
	fi
	
])
