dnl $Id: configure.ac,v 1.8 2005/12/23 00:11:41 erik Exp $

AC_INIT
AC_CONFIG_SRCDIR([src/epac.c])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(epac, 0.4)

AC_PROG_CC
AC_PROG_INSTALL

dnl autosearch /usr/local
AC_MSG_CHECKING([for /usr/local])
if test -d /usr/local/include ; then
        AC_MSG_RESULT([found, adding /usr/local to search paths])
        CPPFLAGS="$CPPFLAGS -I/usr/local/include"
        if test -d /usr/local/lib ; then
                LDFLAGS="$LDFLAGS -L/usr/local/lib"
        fi
else
        AC_MSG_RESULT([not found])
fi

dnl autosearch fink paths
AC_MSG_CHECKING([for fink in /sw])
if test -d /sw/include ; then
        AC_MSG_RESULT([found, adding /sw to search paths])
        CPPFLAGS="$CPPFLAGS -I/sw/include"
        if test -d /sw/lib ; then
                LDFLAGS="$LDFLAGS -L/sw/lib"
        fi
else
        AC_MSG_RESULT([not found])
fi

AC_CHECK_HEADERS(ctype.h dirent.h fcntl.h math.h stdio.h stdlib.h string.h sys/fcntl.h sys/mman.h sys/stat.h sys/types.h sys/uio.h unistd.h,,AC_MSG_ERROR([Missing necessary headers!]))
AC_SEARCH_LIBS(fabs,m,,AC_MSG_ERROR([needs fabs!]))

#COVERAGE_CFLAGS="-fprofile-arcs -ftest-coverage"
#CHECK_CFLAGS=
#CHECK_LIBS=
#have_check=no
#AM_PATH_CHECK(,have_check=yes,AC_MSG_WARN(Not building test suite))
#AM_CONDITIONAL(CHECK, test x$have_check = xyes)
#AC_SUBST(CHECK_CFLAGS)
#AC_SUBST(CHECK_LIBS)
#AC_SUBST(COVERAGE_CFLAGS)

AC_CONFIG_FILES([Makefile src/Makefile epac.spec doxygen.conf])
AC_OUTPUT
