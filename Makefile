CFLAGS+=-pg -ggdb

epac: epac.o
	${CC} ${CFLAGS} -o epac epac.o

epac.o: epac.c
	${CC} ${CFLAGS} -c $<

clean:
	rm -rf epac epac.o
