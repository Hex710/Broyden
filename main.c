#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#include "newton.h"

int main(int argc, char **argv)
{
    long n, max;
    double eps, *x;
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
    x = malloc(sizeof(double) * n);
    fprintf(stdout, "\nValores de x: ");
    for (int i = 0; i < n; i++)
        scanf(&x[i]);

    for (int i = 0; i < max && x != NULL; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(out, "x%d = %f\n", j, x[j]);
        }
        fprintf(out, "#\n");
        x = newton(x, eps, max, n);
    }
}