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

# $Id: Makefile.am,v 1.8 2007/09/05 16:04:26 erik Exp $

bin_PROGRAMS = epac
epac_SOURCES = comp.c display.c epac.c list.c main.c
noinst_HEADERS = comp.h display.h epac.h list.h

EXTRA_DIST = test.sh

indent:
	if [ ! x"`which gindent`" = x ] ; then gindent $(epac_SOURCES) $(noinst_HEADERS) ; else for a in $(epac_SOURCES) $(noinst_HEADERS) ; do indent $$a ; done ; fi

.PHONY : test statictest

test: statictest

statictest: epac
	sh $(top_srcdir)/src/test.sh $(top_srcdir) $(top_builddir)
