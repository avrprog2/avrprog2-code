AC_PREREQ([2.61])
AC_INIT([avrprog2], [1.2.0], [ahagmann@ecs.tuwien.ac.at])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE(foreign [subdir-objects])

AC_CONFIG_SRCDIR([src/CAVRprog.cpp])
AC_CONFIG_HEADERS([config.h])

#AC_LANG_PUSH([C++])
AC_LANG_CPLUSPLUS

# Checks for programs.
AC_PROG_CXX

# doxygen is needed to generate a manpage
AC_CHECK_PROG([DOXYGEN], [doxygen], [yes], [no])
if test $DOXYGEN = no; then
	AC_MSG_ERROR([doxygen was not found])
fi

# dot is needed by doxygen
AC_CHECK_PROG([DOT], [dot], [yes], [no])
if test $DOT = no; then
	AC_MSG_ERROR([dot (graphviz) was not found])
fi

# Checks for libraries and headers

# Need at least libusb-1.0
AC_SEARCH_LIBS([libusb_init], [usb-1.0], [], [AC_MSG_ERROR([libusb-1.0 was not found])])
AC_CHECK_HEADER([libusb-1.0/libusb.h], [], [AC_MSG_ERROR([libusb.h was not found])])

# bfd 
AC_SEARCH_LIBS([bfd_init], [bfd], [], [AC_MSG_ERROR([libbfd was not found])])
AC_CHECK_HEADER([bfd.h], [], [AC_MSG_ERROR([bfd.h was not found])])

# boost-filesystem
AC_CHECK_LIB([boost_filesystem-mt], [main], [], [AC_MSG_ERROR([boost_filesystem library was not found])])
AC_CHECK_HEADER([boost/filesystem.hpp], [], [AC_MSG_ERROR([boost/filesystem.hpp was not found])])

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h string.h])

# boost system
AC_CHECK_LIB([boost_system-mt], [main], [], [AC_MSG_ERROR([boost_system library was not found])])
AC_CHECK_HEADER([boost/foreach.hpp], [], [AC_MSG_ERROR([boost/foreach.hpp was not found])])
AC_CHECK_HEADER([boost/property_tree/ptree.hpp], [], [AC_MSG_ERROR([boost/property_tree/ptree.hpp was not found])])
AC_CHECK_HEADER([boost/property_tree/xml_parser.hpp], [], [AC_MSG_ERROR([boost/property_tree/xml_parser.hpp was not found])])
AC_CHECK_HEADER([boost/algorithm/string.hpp], [], [AC_MSG_ERROR([boost/algorithm/string.hpp was not found])])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([floor memset])

AC_CONFIG_FILES([Makefile Doxyfile Doxyfile_man man.txt])
AC_OUTPUT

