#!/bin/bash

echo 'Running autoscan...'
autoscan

mv configure.scan configure.ac

echo 'Running autoconf...'
autoconf

echo 'Running autoheader...'
autoheader

grep -q 'AM_INIT_AUTOMAKE' configure.ac || \
    sed -i '/AC_INIT(\[FULL-PACKAGE-NAME\], \[VERSION\], \[BUG-REPORT-ADDRESS\])/ a\
AM_INIT_AUTOMAKE(\[FULL-PACKAGE-NAME\], \[VERSION\])\
AM_INIT_AUTOMAKE(\[subdir-objects\])\
' configure.ac

grep -q 'AC_CONFIG_MACRO_DIR(\[m4\])' configure.ac || \
    sed -i '/AM_INIT_AUTOMAKE(\[subdir-objects\])/ a\
AC_CONFIG_MACRO_DIR(\[m4\])' configure.ac

grep -q 'FIND_AVRO' configure.ac || \
    sed -i '/# Checks for libraries\./ a\
FIND_ZWARE_TYPE\
FIND_AVRO\
AC_SUBST(AVRO_PATH)\
FIND_FCGI\
AC_SUBST(FCGI_PATH)\
AC_SUBST(FCGI_CFLAGS)\
AC_SUBST(FCGI_LIBS)\
FIND_LIBMEMCACHED\
AC_SUBST(LIBMEMCACHED_PATH)\
AC_SUBST(LIBMEMCACHED_CFLAGS)\
AC_SUBST(LIBMEMCACHED_LIBS)\
FIND_CJSON\
AC_SUBST(CJSON_INC)\
AC_SUBST(CJSON_LIB)\
' configure.ac

echo 'Running aclocal...'
aclocal

touch COPYING INSTALL NEWS README AUTHORS ChangeLog

echo 'Running automake...'
automake -a -c

echo 'Running autoreconf...'
autoreconf

