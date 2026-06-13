#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#include "likwid.h"
#include "utils.h"
#include "newton.h"

double **montaJacobiana(double *x, long n)
{
    // matriz das jacobianas
    double **jacobianas;
    jacobianas = malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
        jacobianas[i] = calloc(n, sizeof(double));

    // acha o valor das jacobianas
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // matriz das jacobianas eh uma matriz 3-diagonal
            if (i == j)
                jacobianas[i][j] = -4 * x[j] + 3;
            else if (i == j + 1)
                jacobianas[i][j] = -2;
            else if (i == j - 1)
                jacobianas[i][j] = -1;
        }
    }
    return jacobianas;
}

//VERIFICAR USO DE SIMD
double* montaJacobianaOpt(double* x, long n){
    double* jacobiana = calloc(n, sizeof(double)); //tecnicamente malloc seria 1 tico mais eficiente eu acho mas tanto faz
    //Adicionar unrolling
    for (int i=0; i<n; i++){
        jacobiana[i] = -4 * x[i] + 3; //Verificar erro
    }
    return jacobiana;
}

double *achaProxX(double **jacobianas, double *x, double *fx, long n)
{
    // vetor de Δ e de X(i+1)
    double *prox;
    prox = malloc(sizeof(double) * n);

    // metodo de gauss alterado para a matriz 3-diagonal jacobiana
    for (int i = 0; i < (n - 1); i++)
    {
        double m = -2 / jacobianas[i][i];
        jacobianas[i + 1][i + 1] += m;
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
        double y = prox[i + 1] - fx[i];
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

//Pensar sobre como armazenar na memória de forma optimal
double* achaProxXOpt(double* jacobianas, double* x, double* fx, long n){
    double* prox = calloc(n, sizeof(double));
    //ADICIONAR LOOP UNROLLING
    for (int i=0; i<n-1; i++){
        double m = -2/jacobianas[i];
        jacobianas[i+1] += m;
        fx[i+1] -= fx[i] * m;
    }
    prox[n-1] = jacobianas[n-1]/fx[n-1] * -1;

    //LOOP UNROLLING
    for(int i=n-2; i>=0; i--){
        prox[i] = jacobianas[i] / (prox[i+1] - fx[i]);
    }

    //LOOP UNROLLING
    for (int i = 0; i < n; i++)
        prox[i] += x[i];

    return prox;
}

double *newton(double *x0, double eps, long n, rtime_t *jacob, rtime_t *sist)
{
    // vetores de X(i), F(X(i)) e F'(X(i))
    double *xi, *fx, **jacobianas, max = 0.0;
    rtime_t aux = 0;
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
    aux = timestamp();
    jacobianas = montaJacobiana(x0, n);
    *jacob += timestamp() - aux;
    aux = timestamp();
    xi = achaProxX(jacobianas, x0, fx, n);
    *sist += timestamp() - aux;
    // libera o espaco alocado que nao sera mais utilizado
    for (int i = 0; i < n; i++)
        free(jacobianas[i]);
    free(jacobianas);
    free(fx);
    return xi;
}

double* newtonOpt(double *x0, double eps, long n, rtime_t *jacob, rtime_t *sist)
{
    // vetores de X(i), F(X(i)) e F'(X(i))
    double *xi, *fx, *jacobianas, max;
    rtime_t aux = 0;
    fx = malloc(sizeof(double) * n);
    // resolve os sistemas de Broyden para X(0) e acha o maior valor absoluto de fx
    //Primeira linha, tratada a parte
    fx[0] = -2 * x0[0] * x0[0] + 3 * x0[0] - 2 * x0[1] + 1;
    max = fx[0];
    for (int i = 1; i < (n - 1); i++)
    {
        fx[i] = -2 * x0[i] * x0[i] - x0[i - 1] - 2 * x0[i + 1] + 1;
        //REMOVER ESSE IF
        if (fabs(fx[i]) > fabs(max))
            max = fx[i];
    }
    //Ultima linha, tratada a parte
    fx[n - 1] = -2 * x0[n - 1] * x0[n - 1] + 3 * x0[n - 1] - x0[n - 2];
    if (fabs(fx[n - 1]) > max)  max = fx[n - 1];
    // caso de parada com base no valor epsilon
    if (fabs(max) < eps){
        free(fx);
        return NULL;
    }
    // monta as jacobianas para X(0) e acha X(1)
    aux = timestamp();
    jacobianas = montaJacobianaOpt(x0, n);
    *jacob += timestamp() - aux;
    aux = timestamp();
    xi = achaProxXOpt(jacobianas, x0, fx, n);
    *sist += timestamp() - aux;
    // libera o espaco alocado que nao sera mais utilizado
    free(jacobianas);
    free(fx);
    return xi;
}