#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#include "newton.h"

int main(int argc, char **argv)
{
    int sai = argv[1][1];
    long n, max;
    double eps, *x0;
    FILE *out;

    if (sai == 111)
        out = fopen(argv[2], "rb+");
    else
        out = stdout;

    fprintf(stdout, "Dimensao do SNLB: ");
    scanf(&n);
    fprintf(stdout, "\nNumero maximo de iteracoes: ");
    scanf(&max);
    fprintf(stdout, "\nEpsilon: ");
    scanf(&eps);
    x0 = malloc(sizeof(double) * n);
    fprintf(stdout, "\nValores de x0: ");
    for (int i = 0; i < n; i++)
        scanf(&x0[i]);

    newton(x0, eps, max, n);
}