# $Id: Makefile.am,v 1.3 2005/10/27 01:49:14 erik Exp $

bin_PROGRAMS = epac
epac_SOURCES = comp.c display.c epac.c list.c main.c
noinst_HEADERS = comp.h display.h epac.h list.h

indent:
	if [ ! x"`which gindent`" = x ] ; then gindent $(epac_SOURCES) $(noinst_HEADERS) ; else for a in $(epac_SOURCES) $(noinst_HEADERS) ; do indent $$a ; done ; fi

.PHONY : statictest

statictest: epac
	sh test.sh $(top_srcdir) $(top_builddir)/src/statictest
