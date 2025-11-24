#
# Autoconf input file for ZWScene library
#
# Author: Arif

AC_INIT([zwscene], [4.2.0], [arif_mohammad@sigmadesigns.com])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
m4_pattern_allow([AM_PROG_AR])
AM_PROG_AR
AM_SILENT_RULES([yes])
LT_INIT
AC_PROG_CC
AM_PROG_CC_C_O


AC_ARG_WITH([glib],
    AS_HELP_STRING([--with-glib], [Provide path for glib installation]),
    glib_path=${withval},
    glib_path="")

# echo "glib_path is ${glib_path}"    
if [[ -n "${glib_path}" ]]; then
  export PKG_CONFIG_PATH="${glib_path}"/lib/pkgconfig/:$PKG_CONFIG_PATH;
fi
# echo "PKG_CONFIG_PATH is $PKG_CONFIG_PATH"

AC_ARG_WITH([hcapi],
    AS_HELP_STRING([--with-hcapi], [Provide path for HCAPI installation]),
    hcapi_path=${withval},
    hcapi_path="")

# echo "hcapi_path is ${hcapi_path}"    
if [[ -n "${hcapi_path}" ]]; then
  #export CPPFLAGS="-I${hcapi_path}/include -I${hcapi_path}/include/zwave -DOS_LINUX $CPPFLAGS"
  #export CFLAGS="-I${hcapi_path}/include -I${hcapi_path}/include/zwave -DOS_LINUX $CFLAGS"
  AC_SUBST(HCAPI_CPPFLAGS, "-I${hcapi_path}/include -I${hcapi_path}/include/zwave -DOS_LINUX")
  AC_SUBST(HCAPI_CFLAGS, "-I${hcapi_path}/include -I${hcapi_path}/include/zwave -DOS_LINUX")
  AC_SUBST(HCAPI_CPPFLAGS)
  AC_SUBST(HCAPI_CFLAGS)
fi
#dnl # Look for the ZW_classcmd.h and zip_api.h header files.  If not found, terminate.
#AC_CHECK_HEADER(ZW_classcmd.h, [], [AC_MSG_ERROR([Couldn't find or include ZW_classcmd.h])])
#AC_CHECK_HEADER(zip_api.h, [], [AC_MSG_ERROR([Couldn't find or include zip_api.h])])

# echo "CFLAGS is $CFLAGS"
# HACPI headers need OS_LINUX to be defined
# AC_DEFINE([OS_LINUX], [], [HCAPI platform identifier])

AC_ARG_WITH([libesmtp],
    AS_HELP_STRING([--with-libesmtp], [Provide path for libESMTP installation]),
    libesmtp_path=${withval},
    libesmtp_path="")
    
#echo "libesmtp_path is ${libesmtp_path}" 
if [[ -n ${libesmtp_path} ]]; then
	LIBESMTP_CFLAGS=`${libesmtp_path}/bin/libesmtp-config --cflags`
	LIBESMTP_LIBS=`${libesmtp_path}/bin/libesmtp-config --libs`
	#echo "LIBESMTP_CFLAGS: ${LIBESMTP_CFLAGS}"
	#echo "LIBESMTP_LIBS: ${LIBESMTP_LIBS}"
	echo "checking for libESMTP... yes"
	AC_SUBST(LIBESMTP_CFLAGS)
	AC_SUBST(LIBESMTP_LIBS)
fi

AC_ARG_WITH([udunits],
    AS_HELP_STRING([--with-udunits], [Provide path for UDUNITS installation]),
    udunits_path=${withval},
    udunits_path="")

## echo "udunits_path is ${udunits_path}"    
#if [[ -n "${udunits_path}" ]]; then
  #export CPPFLAGS="-I${udunits_path}/include $CPPFLAGS"
  #export CFLAGS="-I${udunits_path}/include $CFLAGS"
#fi
#echo "udunits_path is ${udunits_path}"
#export UDUNITS_CFLAFS="-I${udunits_path}/include"
#export UDUNITS_LIBS="-L${udunits_path}/lib -ludunits2"
#echo "UDUNITS_CFLAFS is ${UDUNITS_CFLAFS}"
#echo "UDUNITS_LIBS is ${UDUNITS_LIBS}"
AC_SUBST(UDUNITS_CFLAFS, "-I${udunits_path}/include")
AC_SUBST(UDUNITS_LIBS, "-L${udunits_path}/lib -ludunits2")

PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.37.93])
AC_SUBST(GLIB_LIBS)
AC_SUBST(GLIB_CFLAGS)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([libzwscene.pc Makefile])
AC_OUTPUT

