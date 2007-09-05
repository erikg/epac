# $Id: Makefile.am,v 1.7 2007/09/05 15:03:19 erik Exp $

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
