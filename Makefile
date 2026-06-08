parametrosCompilacao=-Wall #-Wshadow
nomePrograma=broyden

all: $(nomePrograma)

$(nomePrograma): main.o newton.o utils.o
	gcc -o $(nomePrograma) main.o newton.o utils.o -lm $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

newton.o: newton.h newton.c
	gcc -c newton.c $(parametrosCompilacao)

utils.o: utils.h utils.c
	gcc -c utils.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)