# $Id: Makefile.am,v 1.14 2004/03/03 14:08:51 erik Exp $

SUBDIRS = @TEST@

bin_PROGRAMS = epac
epac_SOURCES = comp.c display.c epac.c list.c main.c
noinst_HEADERS = comp.h display.h epac.h list.h

dist_man_MANS = epac.1
DIST_EXTRA = TODO epac.spec.in

.PHONY : rpm
rpm: dist
	cp $(PACKAGE)-$(VERSION).tar.gz ${HOME}/rpm/SOURCES/
	rpmbuild -ba $(PACKAGE).spec
	cp ${HOME}/rpm/RPMS/i386/$(PACKAGE)-$(VERSION)-*.i386.rpm .
	cp ${HOME}/rpm/SRPMS/$(PACKAGE)-$(VERSION)-*.src.rpm .

.PHONY : deb
deb: 
	debian/rules
	debuild
	debc
	debi
	release

packages: rpm

docs:
	doxygen doxygen.conf
	make -C doxygen/latex 
	dvips -f doxygen/latex/refman.dvi > refman.ps
	ps2pdf refman.ps

clean-local:
	rm -rf doxygen refman.ps refman.pdf

test:
	(cd test && make test)

