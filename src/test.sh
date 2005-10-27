#!/bin/sh

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

#echo $A1 $A2 $A3 $B1 $C1 $D1

test $A1 = $A2 || (echo die "configure != glob" ; exit 1)
test $A1 = $A3 || (echo die "configure != poo/moo" ; exit 1)
test $A1 = $B1 && (echo die "crap, configure == README" ; exit 1)
test $A1 = $C1 && (echo die "crap, configure == configure.ac" ; exit 1)
test $A1 = $D1 && (echo die "crap, configure == poo/Makefile" ; exit 1)

# and clean up
rm -rf $TESTDIR
