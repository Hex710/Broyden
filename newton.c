#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

double **montaJacobiana(double *x, long n)
{
    // matriz das jacobianas
    double **jacobianas;
    jacobianas = malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
        jacobianas[i] = malloc(sizeof(double) * n);

    // jacobianas no começo e no fim da matriz sao um caso aparte
    jacobianas[0][0] = -4 * x[0] + 3;
    jacobianas[0][1] = -2;
    jacobianas[1][0] = -1;
    jacobianas[n - 1][n - 1] = -4 * x[n - 1] + 3;
    jacobianas[n - 2][n - 1] = -2;
    jacobianas[n - 1][n - 2] = -1;
    for (int i = 2; i < n; i++)
    {
        jacobianas[0][i] = 0;
        jacobianas[i][0] = 0;
        jacobianas[n - 1][n - i - 1] = 0;
        jacobianas[n - i - 1][n - 1] = 0;
    }

    // acha o valor das jacobianas entre 0 e n - 1
    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            // matriz das jacobianas eh uma matriz 3-diagonal
            if ((i < j - 1) || (i > j + 1))
                jacobianas[i][j] = 0;
            else if (i = j)
                jacobianas[i][j] = -4 * x[j] + 3;
            else if (i = j + 1)
                jacobianas[i][j] = -2;
            else if (i = j - 1)
                jacobianas[i][j] = -1;
        }
    }
}

double *achaProxX(double **jacobianas, double *x, double *fx, long n);

double *newton(double *x0, double epsilon, long max, long n)
{
    // vetores de X(i), F(X(i)) e F'(X(i))
    double *xi, *fx, **jacobianas;
    xi = malloc(sizeof(double) * n);
    fx = malloc(sizeof(double) * n);
    // resolve os sistemas de Broyden para X(0)
    fx[0] = -2 * x0[0] * x0[0] + 3 * x0[0] - 2 * x0[1] + 1;
    for (int i = 1; i < (n - 1); i++)
        fx[i] = -2 * x0[i] * x0[i] - x0[i - 1] - 2 * x0[i + 1] + 1;
    fx[n - 1] = -2 * x0[n - 1] * x0[n - 1] + 3 * x0[n - 1] - x0[n - 2];
    // monta as jacobianas para X(0) e acha X(1)
    jacobianas = montaJacobiana(x0, n);
    xi = achaProxX(jacobianas, x0, fx, n);
    // repete o processo de solucionar os sistemas de Broyden e achar X(i + 1) para cada iteração até  o máximo.
    for (int i = 1; i < max; i++)
    {
        fx[0] = -2 * xi[0] * xi[0] + 3 * xi[0] - 2 * xi[1] + 1;
        for (int i = 1; i < (n - 1); i++)
            fx[i] = -2 * xi[i] * xi[i] - xi[i - 1] - 2 * xi[i + 1] + 1;
        fx[n - 1] = -2 * xi[n - 1] * xi[n - 1] + 3 * xi[n - 1] - xi[n - 2];
        jacobianas = montaJacobiana(xi, n);
        xi = achaProxX(jacobianas, xi, fx, n);
    }
    return xi;
}