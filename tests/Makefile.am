#/*****************************************************************************
# * Erik's Partial Archive Collator                                           *
# * Copyright (C) 2002-2010 Erik Greenwald <erik@smluc.org>                   *
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

# $Id: Makefile.am,v 1.9 2010/01/06 01:06:53 erik Exp $
#

EXTRA_DIST =
	test.c \
	test_comp.c \
	test_comp.h \
	test_display.c \
	test_display.h \
	test_epac.c \
	test_epac.h \
	test_list.c \
	test_list.h

#noinst_PROGRAMS=test_epac
#epac_src = $(top_srcdir)/src/comp.c $(top_srcdir)/src/display.c $(top_srcdir)/src/epac.c $(top_srcdir)/src/list.c
#tests = test_comp.c test_display.c test_epac.c test_list.c
#test_epac_SOURCES=test.c $(epac_src) $(epac_src:.c=.h) $(tests)
#noinst_HEADERS=$(tests:.c=.h)
#
#AM_CFLAGS = @CHECK_CFLAGS@ @COVERAGE_CFLAGS@ -I$(top_srcdir)/src/
#LDADD  = @CHECK_LIBS@

test: test_epac
	./test_epac

clean-local:
	rm -f *.bb *.bbg *.da *.gcov test.log
