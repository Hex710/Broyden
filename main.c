#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#include "likwid.h"
#include "newton.h"
#include "utils.h"

int main(int argc, char **argv)
{
    long n, max;
    double eps, *x, *aux;
    rtime_t temp, newt, *jacob, *sist;
    FILE *out;

    likwid_markerInit();

    // determina qual vai ser a saida do programa
    if ((argc > 1) && (argv[1][1] == 'o'))
    {
        out = fopen(argv[2], "rb+");
        n = atoi(argv[3]);
    }
    else
    {
        out = stdout;
        n = atoi(argv[2]);
    }
    newt = 0;
    jacob = calloc(1, sizeof(rtime_t));
    sist = calloc(1, sizeof(rtime_t));

    // pede e recebe os valores necessarios para os programas
    max = 25;
    eps = 0.0;
    x = calloc(n, sizeof(double));

    likwid_markerStartRegion("newton");
    // loop principal, ocorre ate que a iteracao maxima seja atingida, ou que a funcao newton chegue em uma condicao de parada (epsilon)
    for (int i = 0; i < max && x != NULL; i++)
    {
        for (int j = 0; j < n; j++)
            fprintf(out, "x%d = %lf\n", j, x[j]);
        fprintf(out, "#\n");
        temp = timestamp();
        aux = newton(x, eps, n, jacob, sist);
        newt += timestamp() - temp;
        free(x);
        x = aux;
    }
    likwid_markerStopRegion("newton");
    fprintf(out, "###########\n# Tempo Total: %lf\n# Tempo Jacobiana: %lf\n# Tempo SL: %lf\n###########\n", newt, *jacob, *sist);

    if ((argc > 1) && (argv[1][1] == 'o'))
        fclose(out);

    free(jacob);
    free(sist);

    likwid_markerClose();
    return 0;
}