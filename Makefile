parametrosCompilacao= -O3 -march=native -mavx -fopt-info-vec
nomePrograma=broyden

all: $(nomePrograma)

$(nomePrograma): main.o newton.o utils.o
	gcc -I/usr/local/include/ -DLIKWID_PERFMON -o $(nomePrograma) main.o newton.o utils.o -lm $(parametrosCompilacao) -L/usr/local/lib -llikwid

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

newton.o: newton.h newton.c
	gcc -c newton.c $(parametrosCompilacao)

utils.o: utils.h utils.c
	gcc -c utils.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)