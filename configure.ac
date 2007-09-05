#/*****************************************************************************
# * Erik's Partial Archive Collator                                           *
# * Copyright (C) 2002-2007 Erik Greenwald <erik@smluc.org>                   *
# *                                                                           *
# * This program takes a directory as an argument, then walks through the     *
# * directory looking for duplicate and partially duplicate files. If it      *
# * an exact duplicate, it deletes one and creates a hardlink to the other,   *
# * minimizing disk usage. If it finds a pair of files where they contain the *
# * same data up to the size of the smaller file, it will prompt if you want  *
# * to combine them. If you say yes, it will delete the smaller of the files  *
# * and hardlink to the larger.                                               *
# *                                                                           *
# * This program is free software; you can redistribute it and/or modify      *
# * it under the terms of the GNU General Public License as published by      *
# * the Free Software Foundation; either version 2 of the License, or         *
# * (at your option) any later version.                                       *
# *                                                                           *
# * This program is distributed in the hope that it will be useful,           *
# * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
# * GNU General Public License for more details.                              *
# *                                                                           *
# * You should have received a copy of the GNU General Public License         *
# * along with this program; if not, write to the Free Software               *
# * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA *
# ****************************************************************************/

dnl $Id: configure.ac,v 1.11 2007/09/05 16:04:26 erik Exp $

AC_INIT
AC_CONFIG_SRCDIR([src/epac.c])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(epac, 0.5)

AC_PROG_CC
AC_PROG_INSTALL

# dnl autosearch /usr/local
# AC_MSG_CHECKING([for /usr/local])
# if test -d /usr/local/include ; then
#         AC_MSG_RESULT([found, adding /usr/local to search paths])
#         CPPFLAGS="$CPPFLAGS -I/usr/local/include"
#         if test -d /usr/local/lib ; then
#                 LDFLAGS="$LDFLAGS -L/usr/local/lib"
#         fi
# else
#         AC_MSG_RESULT([not found])
# fi
# 
# dnl autosearch fink paths
# AC_MSG_CHECKING([for fink in /sw])
# if test -d /sw/include ; then
#         AC_MSG_RESULT([found, adding /sw to search paths])
#         CPPFLAGS="$CPPFLAGS -I/sw/include"
#         if test -d /sw/lib ; then
#                 LDFLAGS="$LDFLAGS -L/sw/lib"
#         fi
# else
#         AC_MSG_RESULT([not found])
# fi

AC_CHECK_HEADERS(ctype.h dirent.h fcntl.h math.h stdio.h stdlib.h string.h sys/fcntl.h sys/mman.h sys/stat.h sys/types.h sys/uio.h unistd.h,,AC_MSG_ERROR([Missing necessary headers!]))
AC_SEARCH_LIBS(fabs,m,,AC_MSG_ERROR([need fabs() from libm!]))

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
