dnl $Id: configure.ac,v 1.3 2004/04/11 14:30:43 erik Exp $

AC_INIT
AC_CONFIG_SRCDIR([epac.c])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(epac, 0.4)

AC_PROG_CC
AC_PROG_INSTALL

AC_CHECK_HEADERS(ctype.h dirent.h fcntl.h math.h stdio.h stdlib.h string.h sys/fcntl.h sys/mman.h sys/stat.h sys/types.h sys/uio.h unistd.h,,AC_MSG_ERROR([Missing necessary headers!]))
AC_SEARCH_LIBS(fabs,m,,AC_MSG_ERROR([needs fabs!]))

COVERAGE_CFLAGS="-fprofile-arcs -ftest-coverage"
CHECK_CFLAGS=
CHECK_LIBS=
have_check=no
AM_PATH_CHECK(,have_check=yes,AC_MSG_WARN(Not building test suite))
AM_CONDITIONAL(CHECK, test x$have_check = xyes)
AC_SUBST(CHECK_CFLAGS)
AC_SUBST(CHECK_LIBS)
AC_SUBST(COVERAGE_CFLAGS)

AC_CONFIG_FILES([Makefile epac.spec doxygen.conf test/Makefile])
AC_OUTPUT
