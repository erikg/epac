# $Id: Makefile.am,v 1.1 2004/03/03 14:08:41 erik Exp $
#

noinst_PROGRAMS=test_epac
epac_src = $(top_srcdir)/comp.c $(top_srcdir)/display.c $(top_srcdir)/epac.c $(top_srcdir)/list.c

test_epac_SOURCES=test_epac.c $(epac_src) $(epac_src:.c=.h)
test_epac_CFLAGS = @CHECK_CFLAGS@
test_epac_LDADD  = @CHECK_LIBS@

test:
	./test_epac
