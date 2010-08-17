#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([gmail2ldif], [0.1], [erikmack@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile src/Makefile])

# Checks for programs.
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_CPP
AC_PROG_RANLIB
AC_PROG_MKDIR_P
AM_PROG_CC_C_O

# Checks for libraries.
# Cygwin (and others certainly) keep iconv elsewhere
have_internal_iconv=yes
AC_CHECK_FUNC(iconv_open,,[have_internal_iconv=no])
if test x${have_internal_iconv} = xno; then
	LIBS=-liconv
	AC_SUBST(LIBS)
fi

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h malloc.h stdlib.h string.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([btowc memmove memset strerror])

AC_OUTPUT
