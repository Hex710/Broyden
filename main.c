#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#include "newton.h"

int main(int argc, char **argv)
{
    long n, max;
    double eps, *x, *aux;
    FILE *out;

    // determina qual vai ser a saida do programa
    if ((argc > 0) && (argv[1][1] == 'o'))
        out = fopen(argv[2], "rb+");
    else
        out = stdout;

    // pede e recebe os valores necessarios para os programas
    fprintf(stdout, "Dimensao do SNLB: ");
    scanf("%ld", &n);
    fprintf(stdout, "\nNumero maximo de iteracoes: ");
    scanf("%ld", &max);
    fprintf(stdout, "\nEpsilon: ");
    scanf("%lf", &eps);
    x = malloc(sizeof(double) * n);
    fprintf(stdout, "\nValores de x: ");
    for (int i = 0; i < n; i++)
        scanf("%lf", &x[i]);

    // loop principal, ocorre ate que a iteracao maxima seja atingida, ou que a funcao newton chegue em uma condicao de parada (epsilon)
    for (int i = 0; i < max && x != NULL; i++)
    {
        for (int j = 0; j < n; j++)
            fprintf(out, "x%d = %lf\n", j, x[j]);
        fprintf(out, "#\n");
        aux = newton(x, eps, n);
        free(x);
        x = aux;
    }

    if ((argc > 0) && (argv[1][1] == 'o'))
        fclose(out);
}