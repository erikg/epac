PACKAGE=epac
VERSION=0.2
DIST=${PACKAGE}-${VERSION}
TAR=${DIST}.tar
BZ=${TAR}.bz2
GZ=${TAR}.gz
ZIP=${DIST}.zip


CFLAGS+=-pg -ggdb

epac: epac.o
	${CC} ${CFLAGS} -o epac epac.o

epac.o: epac.c
	${CC} ${CFLAGS} -c $<

clean:
	rm -rf epac epac.o ${DIST} ${TAR} ${BZ} ${GZ} ${ZIP} 

dist:
	rm -rf ${DIST}
	mkdir ${DIST}
	cp Makefile epac.c LICENSE README ${DIST}/
	chmod 755 ${DIST}
	chmod 644 ${DIST}/*
	tar cvf ${TAR} ${DIST}
	cat ${TAR} | gzip -9 > ${GZ}
	cat ${TAR} | bzip2 -9 > ${BZ}
	zip -9r ${ZIP} ${DIST}
	rm -rf ${TAR} ${DIST}
