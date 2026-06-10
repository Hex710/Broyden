parametrosCompilacao= -O3 -march=native -mavx -fopt-info-vec -L/usr/local/lib -llikwid
nomePrograma=broyden
CFlags=-I/usr/local/include/ -LIKWID_PERFMON

all: $(nomePrograma)

$(nomePrograma): main.o newton.o utils.o
	gcc ${clean} -o $(nomePrograma) main.o newton.o utils.o -lm $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

newton.o: newton.h newton.c
	gcc -c newton.c $(parametrosCompilacao)

utils.o: utils.h utils.c
	gcc -c utils.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)