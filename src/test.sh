#!/bin/sh

# Erik's Partial Archive Collator
# Copyright (C) 2002-2005 Erik Greenwald <erik@smluc.org>
#
# This program takes a directory as an argument, then walks through the
# directory looking for duplicate and partially duplicate files. If it
# an exact duplicate, it deletes one and creates a hardlink to the other,
# minimizing disk usage. If it finds a pair of files where they contain the
# same data up to the size of the smaller file, it will prompt if you want
# to combine them. If you say yes, it will delete the smaller of the files
# and hardlink to the larger.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# $Id: test.sh,v 1.4 2005/10/30 16:44:26 erik Exp $

SRCDIR=$1
DSTDIR=$2
TESTDIR=$DSTDIR

echo $SRCDIR $DSTDIR $TESTDIR

# create a test directory
mkdir $TESTDIR $TESTDIR/poo
cp $SRCDIR/configure.ac $SRCDIR/README $SRCDIR/configure $TESTDIR/
cp $TESTDIR/configure $TESTDIR/glob
cp $TESTDIR/glob $TESTDIR/poo/moo
cp $SRCDIR/src/Makefile $TESTDIR/poo/Makefile

# Ok, so now we have this directory structure
#   README
#   configure
#   configure.ac
#   glob
#   poo/Makefile
#   poo/moo

# run epac
./epac $TESTDIR

# now these files should be the same inode
# configure glob poo/moo

# verify end result

A1=`ls -li $TESTDIR/configure | awk '{print $1}'`
A2=`ls -li $TESTDIR/glob | awk '{print $1}'`
A3=`ls -li $TESTDIR/poo/moo | awk '{print $1}'`
B1=`ls -li $TESTDIR/README | awk '{print $1}'`
C1=`ls -li $TESTDIR/configure.ac | awk '{print $1}'`
D1=`ls -li $TESTDIR/poo/Makefile | awk '{print $1}'`

echo
#echo $A1 $A2 $A3 $B1 $C1 $D1

test $A1 = $A2 || (echo "ERROR: configure != glob")
test $A1 = $A3 || (echo "ERROR: configure != poo/moo")
test $A1 = $B1 && (echo "ERROR: crap, configure == README")
test $A1 = $C1 && (echo "ERROR: crap, configure == configure.ac")
test $A1 = $D1 && (echo "ERROR: crap, configure == poo/Makefile")

# and clean up
rm -rf $TESTDIR
