#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

double achaDeterminante(double **fx, long n)
{
    // caso base para matriz 1 x 1
    if (n == 1)
        return fx[0][0];

    // caso base para matriz 2 x 2
    if (n == 2)
        return (fx[0][0] * fx[1][1] - fx[0][1] * fx[1][0]);

    // caso geral
    double sub[n - 1][n - 1], det = 0;
    for (int i = 0; i < n; i++)
    {
        // monta a submatriz n - 1 x n - 1, pulando a primeira linha e a coluna i
        for (int j = 0; j < n - 1; j++)
        {
            for (int k = 0; k < i; k++)
                sub[j][k] = fx[j + 1][k];
            for (int k = i; k < n - 1; k++)
                sub[j][k] = fx[j + 1][k + 1];
        }
        // se i for ímpar o determinante da submatriz é multiplicado por -1
        if (i % 2 == 0)
            det += fx[i][0] * achaDeterminante(sub, n - 1);
        else
            det -= fx[i][0] * achaDeterminante(sub, n - 1);
    }

    return det;
}

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
    return jacobianas;
}

double *achaProxX(double **jacobianas, double *x, double *fx, long n)
{
    // vetor de X(i+1) e vetor auxiliar para -F(X(i))/J(X(i))
    double *prox, *y;
    prox = malloc(sizeof(double) * n);
    y = malloc(sizeof(double) * n);

    // loop para a equação X(i + 1) = -F(X(i))/J(X(i)) - X(i)
    for (int i = 0; i < n; i++)
    {
        y[i] = 0;
        for (int j = 0; j < n; j++)
        {
            y[i] += fx[j] * jacobianas[i][j] * -1;
        }
        prox[i] = y[i] - x[i];
    }
    free(y);
    return prox;
}

double *newton(double *x0, double eps, long max, long n)
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
    // caso de parada com base no valor epsilon
    if (fabs(achaDeterminante(fx, n)) < eps)
    {
        free(xi);
        free(fx);
        return NULL;
    }
    // monta as jacobianas para X(0) e acha X(1)
    jacobianas = montaJacobiana(x0, n);
    xi = achaProxX(jacobianas, x0, fx, n);
    free(fx);
    return xi;
}