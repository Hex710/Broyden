#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#include "newton.h"

int main(int argc, char **argv)
{
    long n, max;
    double eps, *xi, *xp;
    FILE *out;

    if (argv[1][1] == 'o')
        out = fopen(argv[2], "rb+");
    else
        out = stdout;

    fprintf(stdout, "Dimensao do SNLB: ");
    scanf(&n);
    fprintf(stdout, "\nNumero maximo de iteracoes: ");
    scanf(&max);
    fprintf(stdout, "\nEpsilon: ");
    scanf(&eps);
    xi = malloc(sizeof(double) * n);
    xp = malloc(sizeof(double) * n);
    fprintf(stdout, "\nValores de x: ");
    for (int i = 0; i < n; i++)
        scanf(&xp[i]);

    // vou perguntar pro professor como que eh para lidar com o epsilon ja que precisa fazer todas as iteracoes

    for (int i = 0; i < max; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(out, "x%d = %f\n", j, xp[j]);
            xi[j] = xp[j];
        }
        fprintf(out, "#\n");
        xp = newton(xi, max, n);
        // inserir a parada por epsilon
    }
}