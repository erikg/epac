dnl $Id: configure.ac,v 1.1 2004/04/11 13:51:55 erik Exp $

AC_INIT
AC_CONFIG_SRCDIR([epac.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(epac, 0.4)

AC_PROG_CC
AC_PROG_INSTALL

AC_CHECK_HEADERS(ctype.h dirent.h fcntl.h math.h stdio.h stdlib.h string.h sys/fcntl.h sys/mman.h sys/stat.h sys/types.h sys/uio.h unistd.h,,AC_MSG_ERROR([Missing necessary headers!]))
AC_SEARCH_LIBS(fabs,m,,AC_MSG_ERROR([needs fabs!]))

COVERAGE_CFLAGS=
AM_PATH_CHECK(,[
		TEST=test
		COVERAGE_CFLAGS="-fprofile-arcs -ftest-coverage"
		] ,TEST=)
AC_SUBST(CHECK_CFLAGS)
AC_SUBST(CHECK_LIBS)
AC_SUBST(TEST)
AC_SUBST(COVERAGE_CFLAGS)

AC_CONFIG_FILES([Makefile epac.spec doxygen.conf test/Makefile])
AC_OUTPUT
