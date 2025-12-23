CC = clang
CFLAGS = -std=c11 -Wall -Wextra -pedantic -O3 -g0 # -Werror
CLI = lambda
ART = ${CLI} *.o *.out.*

.PHONY: c all r

all: r

lambda: main.c
	${CC} ${CFLAGS} ${XFLAGS} $^ -o $@

c:
	rm -f ${ART}

r: c ${CLI}
	./${CLI} "(app (abs x x) y)"
