INCS=
LIBS=

all:
	@${CC} -g -O0 -Wall --warn-unused-variable ${INCS} -o hsort hsort.c tests.c ${LIBS}
