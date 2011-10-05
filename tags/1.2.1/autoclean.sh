#!/bin/sh

# deletes anything created by automake, autoconf and ./configure

make distclean 2> /dev/null
make -k maintainer-clean 2> /dev/null
rm -rf build-aux
rm -f Makefile.in
rm -f config.h.in
rm -f *.m4
rm -f *.log
rm -rf autom4te.cache
rm -f configure missing install-sh depcomp
