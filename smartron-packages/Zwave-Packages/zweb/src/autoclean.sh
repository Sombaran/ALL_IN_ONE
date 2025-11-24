#!/bin/sh

make distclean || true

rm -rf autom4te.cache autoscan.log config.h.in* config.log config.status configure configure.ac Makefile.in aclocal.m4 stamp-h1 config.h Makefile
rm -f COPYING depcomp INSTALL install-sh missing
rm -f INSTALL NEWS README AUTHORS ChangeLog

rm -f config.guess config.sub ltmain.sh

rm -f libtool

