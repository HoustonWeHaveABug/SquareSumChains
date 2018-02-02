SQUARE_SUM_CHAINS_C_FLAGS=-O2 -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

square_sum_chains: square_sum_chains.o
	gcc -o square_sum_chains square_sum_chains.o

square_sum_chains.o: square_sum_chains.c square_sum_chains.make
	gcc -c ${SQUARE_SUM_CHAINS_C_FLAGS} -o square_sum_chains.o square_sum_chains.c

clean:
	rm -f square_sum_chains square_sum_chains.o
