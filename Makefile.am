#/*****************************************************************************
# * Erik's Partial Archive Collator                                           *
# * Copyright (C) 2002-2008 Erik Greenwald <erik@smluc.org>                   *
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

# $Id: Makefile.am,v 1.24 2008/02/01 20:08:53 erik Exp $

#if CHECK
#TESTDIR = tests
#endif

SUBDIRS = src tests debian

dist_man_MANS = epac.1
EXTRA_DIST = TODO epac.spec.in

.PHONY : rpm
rpm: dist
	cp $(PACKAGE)-$(VERSION).tar.gz ${HOME}/rpm/SOURCES/
	rpmbuild -ba $(PACKAGE).spec
	cp ${HOME}/rpm/RPMS/i386/$(PACKAGE)-$(VERSION)-*.i386.rpm .
	cp ${HOME}/rpm/SRPMS/$(PACKAGE)-$(VERSION)-*.src.rpm .

.PHONY : deb
deb: dist
	rm -rf $(PACKAGE)-$(VERSION)
	tar zxvf $(PACKAGE)-$(VERSION).tar.gz
	(cd $(PACKAGE)-$(VERSION) && fakeroot debian/rules binary)
	(cd $(PACKAGE)-$(VERSION) && debuild -us -uc)
	rm -rf $(PACKAGE)-$(VERSION)

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

