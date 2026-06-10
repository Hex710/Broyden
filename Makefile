parametrosCompilacao= -O3 -march=native -mavx -fopt-info-vec
nomePrograma=broyden

all: $(nomePrograma)

$(nomePrograma): main.o newton.o utils.o
	gcc -I /usr/local/include/ -L /usr/local/lib -DLIKWID_PERFORM -o $(nomePrograma) main.o newton.o utils.o -lm $(parametrosCompilacao) -llikwid

main.o: main.c
	gcc -c main.c -DLIKWID_PERFORM $(parametrosCompilacao)

newton.o: newton.h newton.c
	gcc -c newton.c $(parametrosCompilacao)

utils.o: utils.h utils.c
	gcc -c utils.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)