# $Id: Makefile.am,v 1.4 2004/04/11 15:20:22 erik Exp $
#

noinst_PROGRAMS=test_epac
epac_src = $(top_srcdir)/src/comp.c $(top_srcdir)/src/display.c $(top_srcdir)/src/epac.c $(top_srcdir)/src/list.c
tests = test_comp.c test_display.c test_epac.c test_list.c
test_epac_SOURCES=test.c $(epac_src) $(epac_src:.c=.h) $(tests)
noinst_HEADERS=$(tests:.c=.h)

AM_CFLAGS = @CHECK_CFLAGS@ @COVERAGE_CFLAGS@ -I$(top_srcdir)/src/
LDADD  = @CHECK_LIBS@

test: test_epac
	./test_epac

clean-local:
	rm -f *.bb *.bbg *.da test.log
