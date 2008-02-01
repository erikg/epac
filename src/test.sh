#!/bin/sh

# Erik's Partial Archive Collator
# Copyright (C) 2002-2008 Erik Greenwald <erik@smluc.org>
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

# $Id: test.sh,v 1.12 2008/02/01 20:08:53 erik Exp $

SRCDIR=$1
DSTDIR=$2
TESTDIR=$DSTDIR/test$$

echo $SRCDIR $DSTDIR $TESTDIR

# create a test directory
mkdir -p $TESTDIR/poo
cp $SRCDIR/configure.ac $SRCDIR/README $SRCDIR/configure $TESTDIR/
cp $TESTDIR/configure $TESTDIR/glob
cp $TESTDIR/glob $TESTDIR/poo/moo
dd if=$SRCDIR/src/Makefile.in of=$TESTDIR/zappa bs=1k count=1
dd if=$SRCDIR/src/Makefile.in of=$TESTDIR/frank bs=1k count=2
dd if=$SRCDIR/src/Makefile.in of=$TESTDIR/290 bs=1 count=290
dd if=$SRCDIR/src/Makefile.in of=$TESTDIR/300 bs=1 count=300
dd if=$SRCDIR/src/Makefile.in of=$TESTDIR/310 bs=1 count=310
dd if=$SRCDIR/src/Makefile.in of=$TESTDIR/320 bs=1 count=320
cp $SRCDIR/src/Makefile.in $TESTDIR/poo/Makefile.in
ln $TESTDIR/configure.ac $TESTDIR/meep
ln $TESTDIR/configure.ac $TESTDIR/poo/furk

# Ok, so now we have this directory structure
#   300
#   310
#   320
#   290
#   README
#   configure
#   configure.ac
#   frank
#   glob
#   meep (same as glob)
#   poo/Makefile.in
#   poo/furk (same as glob)
#   poo/moo
#   zappa

# run epac
yes | ./epac $TESTDIR

# now these files should be the same inode
# configure glob poo/moo
# configure.ac meep poo/furk

# verify end result

A1=`ls -li $TESTDIR/configure | awk '{print $1}'`
A2=`ls -li $TESTDIR/glob | awk '{print $1}'`
A3=`ls -li $TESTDIR/poo/moo | awk '{print $1}'`
B1=`ls -li $TESTDIR/README | awk '{print $1}'`
C1=`ls -li $TESTDIR/configure.ac | awk '{print $1}'`
C2=`ls -li $TESTDIR/meep | awk '{print $1}'`
C3=`ls -li $TESTDIR/poo/furk | awk '{print $1}'`
D1=`ls -li $TESTDIR/poo/Makefile.in | awk '{print $1}'`
D2=`ls -li $TESTDIR/frank | awk '{print $1}'`
D3=`ls -li $TESTDIR/zappa | awk '{print $1}'`
D4=`ls -li $TESTDIR/290 | awk '{print $1}'`
D5=`ls -li $TESTDIR/300 | awk '{print $1}'`
D6=`ls -li $TESTDIR/310 | awk '{print $1}'`
D7=`ls -li $TESTDIR/320 | awk '{print $1}'`

find $TESTDIR -type f | xargs ls -li | sort -n

echo
echo $A1 $A2 $A3 | xargs -n 1 | sort -n | xargs
echo $B1 
echo $C1 $C2 $C3 | xargs -n 1 | sort -n | xargs
echo $D1 $D2 $D3 $D4 $D5 $D6 $D7 | xargs -n 1 | sort -n | xargs

test $A1 = $A2 || (echo "ERROR: configure != glob")
test $A1 = $A3 || (echo "ERROR: configure != poo/moo")
test $A1 = $B1 && (echo "ERROR: crap, configure == README")
test $A1 = $C1 && (echo "ERROR: crap, configure == configure.ac")
test $A1 = $D1 && (echo "ERROR: crap, configure == poo/Makefile.in")

# and clean up
rm -rf $TESTDIR
