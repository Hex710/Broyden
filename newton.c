#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#include "newton.h"

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
            else if (i == j)
                jacobianas[i][j] = -4 * x[j] + 3;
            else if (i == j + 1)
                jacobianas[i][j] = -2;
            else if (i == j - 1)
                jacobianas[i][j] = -1;
        }
    }
    return jacobianas;
}

double *achaProxX(double **jacobianas, double *x, double *fx, long n)
{
    // vetor de X(i+1)
    double *prox;
    prox = malloc(sizeof(double) * n);

    // metodo de gauss alterado para a matriz 3-diagonal jacobiana
    for (int i = 0; i < (n - 1); i++)
    {
        double m = -2 / jacobianas[i][i];
        jacobianas[i + 1][i + 1] -= -1 * m;
        fx[i + 1] -= fx[i] * m;
    }
    // acha Δ em J(X) * Δ = -F(X)
    // teste para impedir divisao por 0
    if (fx[n - 1] == 0)
        prox[n - 1] = 0;
    else
        prox[n - 1] = jacobianas[n - 1][n - 1] / fx[n - 1] * -1;
    for (int i = n - 2; i >= 0; i--)
    {
        double y = (fx[i] * -1) + prox[i + 1];
        if (y == 0)
            prox[i] = 0;
        else
            prox[i] = jacobianas[i][i] / y;
    }
    // acha X(i+1) em X(i+1) = Δ + X(i)
    for (int i = 0; i < n; i++)
        prox[i] += x[i];

    return prox;
}

double *newton(double *x0, double eps, long n)
{
    // vetores de X(i), F(X(i)) e F'(X(i))
    double *xi, *fx, **jacobianas, max = 0.0;
    fx = malloc(sizeof(double) * n);
    // resolve os sistemas de Broyden para X(0) e acha o maior valor absoluto de fx
    fx[0] = -2 * x0[0] * x0[0] + 3 * x0[0] - 2 * x0[1] + 1;
    if (fabs(fx[0]) > max)
        max = fx[0];
    for (int i = 1; i < (n - 1); i++)
    {
        fx[i] = -2 * x0[i] * x0[i] - x0[i - 1] - 2 * x0[i + 1] + 1;
        if (fabs(fx[i]) > fabs(max))
            max = fx[i];
    }
    fx[n - 1] = -2 * x0[n - 1] * x0[n - 1] + 3 * x0[n - 1] - x0[n - 2];
    if (fabs(fx[n - 1]) > max)
        max = fx[n - 1];
    // caso de parada com base no valor epsilon
    if (fabs(max) < eps)
    {
        free(fx);
        return NULL;
    }
    // monta as jacobianas para X(0) e acha X(1)
    jacobianas = montaJacobiana(x0, n);
    xi = achaProxX(jacobianas, x0, fx, n);
    // libera o espaco alocado que nao sera mais utilizado
    for (int i = 0; i < n; i++)
        free(jacobianas[i]);
    free(jacobianas);
    free(fx);
    return xi;
}