# $Id: Makefile.am,v 1.2 2004/02/22 20:08:01 erik Exp $
#

noinst_PROGRAMS=test_epac
epac_src = $(top_srcdir)/comp.c $(top_srcdir)/display.c $(top_srcdir)/epac.c $(top_srcdir)/list.c
tests = test_comp.c test_display.c test_epac.c test_list.c
test_epac_SOURCES=test.c $(epac_src) $(epac_src:.c=.h) $(tests)
noinst_HEADERS=$(tests:.c=.h)

test_epac_CFLAGS = @CHECK_CFLAGS@ @COVERAGE_CFLAGS@
test_epac_LDADD  = @CHECK_LIBS@

test: test_epac
	./test_epac

clean-local:
	rm -f *.bb *.bbg test.log
