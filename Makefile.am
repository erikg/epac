# $Id: Makefile.am,v 1.21 2005/12/04 18:45:07 erik Exp $

#if CHECK
#TESTDIR = tests
#endif

SUBDIRS = src #$(TESTDIR)

dist_man_MANS = epac.1
EXTRA_DIST = TODO epac.spec.in

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
	(cd src && make statictest)
#	(cd tests && make test)

