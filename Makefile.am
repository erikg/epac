# $Id: Makefile.am,v 1.9 2003/12/27 18:28:49 erik Exp $

bin_PROGRAMS = epac
epac_SOURCES = comp.c display.c epac.c list.c
noinst_HEADERS = comp.h display.h epac.h list.h

dist_man_MANS = epac.1
DIST_EXTRA = TODO epac.spec.in

.PHONY : rpm
rpm: dist
	cp $(PACKAGE)-$(VERSION).tar.gz /usr/src/redhat/SOURCES/
	rpm -ba data/$(PACKAGE).spec
	cp /usr/src/redhat/RPMS/i386/$(PACKAGE)-$(VERSION)-*.i386.rpm .
	cp /usr/src/redhat/SRPMS/$(PACKAGE)-$(VERSION)-*.src.rpm .

.PHONY : deb
deb: 
	debian/rules
	debuild
	debc
	debi
	release

packages: rpm

