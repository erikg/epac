# $Id: Makefile.am,v 1.2 2004/04/11 16:40:50 erik Exp $

bin_PROGRAMS = epac
epac_SOURCES = comp.c display.c epac.c list.c main.c
noinst_HEADERS = comp.h display.h epac.h list.h

indent:
	if [ ! x"`which gindent`" = x ] ; then gindent $(epac_SOURCES) $(noinst_HEADERS) ; else for a in $(epac_SOURCES) $(noinst_HEADERS) ; do indent $$a ; done ; fi
